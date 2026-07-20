// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/stdlib.h>
#include <libk/stdbool.h>
#include <libk/string.h>
#include <libk/stdio.h>
#include <mm/pmmgr.h>
#include <mm/vmmgr.h>
#include <mm/bios_mmap.h>
#include <mm/heap.h>
#include <mm/layout.h>
#include <kernel/idt.h>
#include <kernel/debug.h>
#include <kernel/utils.h>
#include <kernel/input/input.h>
#include <kernel/textbuffer.h>
#include <kernel/console.h>
#include <kernel/tty.h>
#include <kernel/sched.h>
#include <kernel/klog.h>
#include <drivers/isa/pic.h>
#include <drivers/isa/pit.h>
#include <drivers/isa/uart.h>
#include <drivers/pci/pcie.h>
#include <drivers/acpi/acpi.h>
#include <drivers/acpi/hpet.h>
#include <drivers/acpi/ioapic.h>
#include <drivers/video/vga.h>
#include <drivers/input/keyboard.h>
#include <drivers/pci/edu.h>
#include <drivers/isa/ata.h>

#include "banner.h"

// C entrypoint
int kernel_main(void)
{
  // Sanity checks
  // -------------

  vga_init();
  if (vga_get_memory_map() != 0x3) {
    vga_clear(VGA_STYLE_RED);
    vga_print(0, "Memory map range not supported", VGA_STYLE_BW);
    return EXIT_FAILURE;
  }

  if (!vga_is_text_mode()) {
    vga_clear(VGA_STYLE_RED);
    vga_print(0, "VGA alphanumeric mode is disabled", VGA_STYLE_BW);
    return EXIT_FAILURE;
  }

  if (!uart_init_port(&uart_port1)) {
    vga_clear(VGA_STYLE_RED);
    vga_print(0, "Error initializing uart", VGA_STYLE_BW);
    return EXIT_FAILURE;
  }

  // Checks successfull

  // Setup memory management
  // -----------------------

  debug_print_memory_map_uart();

  pmmgr_init();
  // debug_print_pmmgr_bitfield();

  struct vmmgr vmmgr;
  // Identity map the first 4MB of memory
  vmmgr_setup(&vmmgr);
  vmmgr_activate(&vmmgr);
  // debug_print_pmmgr_bitfield();

  size_t heap_size = 4 * 1024 * 1024;
  heap_init(&vmmgr, heap_size);
  void* some_mem = kmalloc(1024);
  (void) some_mem;
  // debug_print_pmmgr_bitfield();

  // Setup interrupts
  // ----------------

  pit_set_count(1193); // one tick per millisecond

  pic_remap();  // Chage overlapping IRQ numbers

  idt_set();    // Setup the IDT

  // Setup devices
  // -------------
  //
  // We do this manually until the device model fully implemented

  void* hpet_base   = NULL;

  struct acpi_rsdp* acpi_rsdp = acpi_locate_rsdp();
  if (!acpi_rsdp) {
    kerr("Could not find ACPI RSDP table\n");
  } else {
    kinfo("Found ACPI RSDP at: %x\n", acpi_rsdp);

    struct page_entry_flags page_flags = { .rw = 1 };

    // HPET setup

    struct hpet_acpi_sdt *hpet =
      acpi_locate_sdt(acpi_rsdp, HPET_ACPI_SIGNATURE);

    if (!hpet) {
      kerr("Could not find HPET timer\n");
    } else {
      kinfo("Found HPET timer, register at address: %x\n", hpet->address);

      // Identity map
      hpet_base = MM_PHYS_TO_VIRT(hpet->address);
      paging_add_entry((void*)hpet->address, hpet_base, page_flags);

      hpet_enable(hpet_base, false);
    }

    // IOAPIC

    struct ioapic_acpi_sdt *ioapic_base =
      acpi_locate_sdt(acpi_rsdp, IOAPIC_ACPI_SIGNATURE);
    if (!ioapic_base) {
      kerr("Could not find I/O APIC from ACPI\n");
    } else {
      kinfo("Found I/O APIC from ACPI\n");

      struct ioapic_record_header* record_it = &ioapic_base->records[0];

      while ((u64_t)record_it < (u64_t)ioapic_base + ioapic_base->header.length) {
        u8_t type = record_it->entry_type;
        switch (type)
        {
        case 1: {
          struct ioapic_record_ioapic *ioapic_struct = (void*)record_it;
          kinfo("[ioapic] Found an APIC Structure with id %d, addr %x\n",
                ioapic_struct->ioapic_id, ioapic_struct->ioapic_addr);
          break;
        }
        default:
          break;
        }

        u8_t *it_bytes = (void*)record_it;
        it_bytes += record_it->length;
        record_it = (void*)it_bytes;
      }
    }
  }

  // PCIe

  struct pcie_acpi_sdt* pcie_sdt = acpi_locate_sdt(acpi_rsdp, PCIE_ACPI_SIGNATURE);
  if (!pcie_sdt) {
    kerr("Could not locate PCIe sdt\n");
  } else {
    kinfo("Found PCIe sdt\n");
    // debug_dump_mem_page(pcie_sdt->entries[0].base_addr, 32);
    debug_enumerate_pcie_devices(pcie_sdt);

    struct edu_device edu;
    if (edu_init(&edu, pcie_sdt)) {
      u32_t edu_id = edu_read_identification(&edu);

      kinfo("[edu] Initialized EDU device\n");
      kinfo("[edu] identification: %x\n", edu_id);

      edu_int_raise(&edu);
    } else {
      kerr("error initializing EDU device\n");
    }
  }

  if (ata_enabled(ATA_BUS1_BASE_PORT)) {
    kerr("error ATA port\n");
  } else {
    u8_t ata_buff[ATA_SECTOR_SIZE] = {0};
    if (!ata_read(ATA_BUS1_BASE_PORT, ata_buff, 5, 1)) {
      kerr("error reading from ATA drive\n");
    } else {
      kinfo("ATA read: %s\n", ata_buff);
    }
  }

  // Tests  
  debug_print_vga();
  debug_write_uart();
  debug_enumerate_pci_devices();
  debug_print_true_rand();
  debug_print_true_rand();
  debug_print_true_rand();
  debug_dump_regs_uart();
  debug_dump_regs_uart2();
  // breakpoint();

  // Setup tty
  // ---------

  vga_init();

  struct textbuffer_entry buff[VGA_TEXT_BUFFER_SIZE] = {0};
  struct textbuffer textbuffer;
  textbuffer_init(&textbuffer, buff,
                  VGA_TEXT_BUFFER_WIDTH, VGA_TEXT_BUFFER_HEIGHT, 0, 0);

  struct tty tty;
  tty_init(&tty, &textbuffer, TEXTBUFFER_STYLE_BW, &glob_vga_console);

  struct input input;
  input_init(&input, &input_keymap_us, &tty);

  struct keyboard keyboard;
  keyboard_init(&keyboard, KEYBOARD_TYPE_PS2_SET1, &input);
  keyboard_set_active(&keyboard);

  tty_write(&tty, povos_banner, strlen(povos_banner));
  tty_flush(&tty);

  //vga_set_graphics_mode();
  //debug_vga_draw_flag();
  //debug_sleep();
  //reboot();

  // Scheduler
  // ---------

  // Test Task A
  u64_t *stack_top_a = (u64_t*)((u64_t)kmalloc(4096) + 4096);
  struct cpu_regs regs_a;
  regs_save(&regs_a);
  regs_a.rip = (u64_t)(void*)debug_test_task_a_fn;
  regs_a.rsp = (u64_t)stack_top_a;

  // Test Task B
  u64_t *stack_top_b = (u64_t*)((u64_t)kmalloc(4096) + 4096);
  struct cpu_regs regs_b;
  regs_save(&regs_b);
  regs_b.rip = (u64_t)(void*)debug_test_task_b_fn;
  regs_b.rsp = (u64_t)stack_top_b;

  struct task task_a = task_create(regs_a, &vmmgr, "task A");
  struct task task_b = task_create(regs_b, &vmmgr, "task B");

  sched_init(&vmmgr);
  sched_start_task(task_a);
  sched_start_task(task_b);

  while(1) {
    char c = uart_getc(uart_port1);
    if (c == '\r') c = '\n';
    printk("%c", c);
  }

  // Read and print keyboard input
  //debug_dump_input_loop(&input, (void*)hpet_base);

  sched_loop();

  return EXIT_SUCCESS;
}
