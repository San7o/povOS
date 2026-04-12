// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/unistd.h>
#include <kernel/debug.h>   // implements
#include <kernel/mm/paging.h>
#include <kernel/time.h>
#include <drivers/uart.h>
#include <drivers/video/vga.h>
#include <drivers/pci.h>
#include <drivers/hpet.h>
#include <kernel/mm/bios_mmap.h>
#include <kernel/mm/pmmgr.h>
#include <kernel/mm/layout.h>
#include <kernel/utils.h>

void debug_dump_regs_uart3(cpu_regs_t regs)
{
#ifdef __x86_64__
  uart_printf(uart_port1, "[debug] [regs2] rax: %x\n", regs.rax);
  uart_printf(uart_port1, "[debug] [regs2] rbx: %x\n", regs.rbx);
  uart_printf(uart_port1, "[debug] [regs2] rcx: %x\n", regs.rcx);
  uart_printf(uart_port1, "[debug] [regs2] rdx: %x\n", regs.rdx);
  uart_printf(uart_port1, "[debug] [regs2] rsi: %x\n", regs.rsi);
  uart_printf(uart_port1, "[debug] [regs2] rdi: %x\n", regs.rdi);
  uart_printf(uart_port1, "[debug] [regs2] rsp: %x\n", regs.rsp);
  uart_printf(uart_port1, "[debug] [regs2] rbp: %x\n", regs.rbp);
  uart_printf(uart_port1, "[debug] [regs2] r8:  %x\n", regs.r8);
  uart_printf(uart_port1, "[debug] [regs2] r9:  %x\n", regs.r9);
  uart_printf(uart_port1, "[debug] [regs2] r10: %x\n", regs.r10);
  uart_printf(uart_port1, "[debug] [regs2] r11: %x\n", regs.r11);
  uart_printf(uart_port1, "[debug] [regs2] r12: %x\n", regs.r12);
  uart_printf(uart_port1, "[debug] [regs2] r13: %x\n", regs.r13);
  uart_printf(uart_port1, "[debug] [regs2] r14: %x\n", regs.r14);
  uart_printf(uart_port1, "[debug] [regs2] r15: %x\n", regs.r15);
  uart_printf(uart_port1, "[debug] [regs2] rip: %x\n", regs.rip);
  uart_printf(uart_port1, "[debug] [regs2] rflags: %x\n", regs.rflags);
  uart_printf(uart_port1, "[debug] [regs2] cr3: %x\n",    regs.cr3);
#endif
}

void debug_dump_regs_uart2(void)
{
  cpu_regs_t regs;
  regs_save(&regs);
  debug_dump_regs_uart3(regs);
}

void debug_dump_mem_page(phys_addr_t phys_mem, size_t count)
{
  page_entry_flags_t page_flags = { .rw = 1 };

  // Map a virtual address to rsdt_phys
  // We can identity map this
  void* virt_mem = (void*)phys_mem;
  paging_add_entry((void*)phys_mem, virt_mem, page_flags);

  u64_t* mem = virt_mem;
  uart_printf(uart_port1, "[debug] Memory at phys addr %x:",
              phys_mem, virt_mem);
  for (unsigned int i = 0; i < PAGE_SIZE / sizeof(u64_t) && i < count; ++i)
  {
    if (i % 4 == 0)
    {
      uart_printf(uart_port1, "\n[debug] [%x] ", &mem[i]);
    }

    uart_putc(uart_port1, ' ');
    uart_write_hex(uart_port1, mem[i]);   // CPU triple faults immediately
  }

  uart_putc(uart_port1, '\n');
}

void debug_dump_keyboard_event_uart(keyboard_event_t event)
{
  const char* key_str = keycode_to_string(event.key);

  uart_printf(uart_port1, "[debug] [keyboard_event] %s %s\n",
              (event.pressed) ? "pressed  " : "released ", key_str);

  return;
}

void debug_dump_input_event_uart(input_event_t event)
{
  uart_write_str(uart_port1, "[debug] [input_event] ");

  switch (event.type)
  {
  case INPUT_EVENT_TYPE_KEYBOARD: ;
    const char* key_str = keycode_to_string(event.e.key.key);
    uart_write_str(uart_port1, (event.e.key.pressed) ? "pressed " : "released ");
    uart_write_str(uart_port1, key_str);
    break;
  case INPUT_EVENT_TYPE_CHAR:
    uart_putc(uart_port1, event.e.c);
    break;
  default:
    uart_write_str(uart_port1, "unknown");
    break;
  }
  
  uart_putc(uart_port1, '\n');
  return;
}

void debug_print_vga(void)
{
  vga_clear(VGA_STYLE_BLUE);
  vga_print(0, "[debug] Hello, from povOS!", VGA_STYLE_BLUE);
  vga_print_hex(18, 0x6969, VGA_STYLE_BW);
  return;
}

void debug_write_uart(void)
{
  uart_printf(uart_port1, "[debug] Hello, from povOS! %x\n", 0x6969);
  return;
}

void debug_dump_input_loop(input_t *input, void* hpet_base_reg)
{
  u64_t previous_time_s = 0;
  while(1) {

    // Tick seconds
    if (time_ms / 1000 > previous_time_s)
    {
      previous_time_s = time_ms / 1000;

      uart_printf(uart_port1, "[debug] [isr] [pit 0] time: %d s\n", time_ms / 1000);

      if (hpet_base_reg)
      {
        u64_t hpet_counter = hpet_poll(hpet_base_reg);
        uart_printf(uart_port1, "[debug] [hpet] counter: %d\n", hpet_counter);
      }
    }
    
    input_event_t event = input_events_get(input);
    if (event.type == INPUT_EVENT_TYPE_NONE) continue;
    
    debug_dump_input_event_uart(event);

  }
}

void debug_print_memory_map_uart(void)
{
  u32_t *mmap_num_entries = BIOS_MMAP_NUM_ENTRIES_ADDR;
  bios_mmap_entry_t *mmap = BIOS_MMAP_ENTRIES_ADDR;
  if (!mmap_num_entries || !mmap) return;
  
  uart_write_str(uart_port1, "[debug] [memory map] Memory map entries: ");
  uart_write_hex(uart_port1, (u64_t) *mmap_num_entries);
  uart_putc(uart_port1, '\n');

  for (u32_t i = 0; i < *mmap_num_entries && i < 50; ++i)
  {
    u64_t base = (u64_t)mmap[i].base_low | ((u64_t) mmap[i].base_high << 32);
    uart_write_str(uart_port1, "[debug] [mmap] base: ");
    uart_write_hex(uart_port1, base);

    u64_t length = (u64_t)mmap[i].length_low | ((u64_t) mmap[i].base_high << 32);
    uart_write_str(uart_port1, ", length: ");
    uart_write_hex(uart_port1, length);

    uart_write_str(uart_port1, ", type: ");
    uart_write_hex(uart_port1, (u64_t) mmap[i].type);
    
    uart_write_str(uart_port1, ", acpi: ");
    uart_write_hex(uart_port1, (u64_t) mmap[i].acpi);

    uart_putc(uart_port1, '\n');
  }
  
  return;
}

void debug_print_pmmgr_bitfield(void)
{
  uart_printf(uart_port1, "[debug] [pmmgr] Bitfield of size %d:\n",
              pmmgr.size);
  uart_write_str(uart_port1, "[debug] [pmmgr] [0x0000000000000000] ");
  
  for (u64_t i = 0; i < pmmgr.size; ++i)
  {
    for (int bit = 0; bit < 8; ++bit)
    {
      int val = (pmmgr.bitfield[i] >> bit) & 1;
      uart_write_str(uart_port1, (val == 1) ? "1" : "0");      
    }

    if ((i + 1) % 8 == 0)
    {
      uart_write_str(uart_port1, "\n");
      uart_write_str(uart_port1, "[debug] [pmmgr] [");
      uart_write_hex(uart_port1, (i + 1) * 8 * PAGE_SIZE);
      uart_write_str(uart_port1, "] ");
    }
  }
  
  uart_putc(uart_port1, '\n');
}

void debug_vga_draw_flag(void)
{
  for (int x = 0; x < 320 / 3; ++x)
    for (int y = 0; y < 200; ++y)    
      vga_draw_pixel(x, y, VGA_COLOR_GREEN);
  for (int x = 0; x < 320 / 3; ++x)
    for (int y = 0; y < 200; ++y)    
      vga_draw_pixel(320 / 3 + x, y, VGA_COLOR_WHITE);
  for (int x = 0; x < 320 / 3; ++x)
    for (int y = 0; y < 200; ++y)    
      vga_draw_pixel(320 / 3 * 2 + x, y, VGA_COLOR_RED);
}

void debug_enumerate_pci_devices(void)
{
  for (int bus = 0; bus < 256; bus++)
  {
    for (int slot = 0; slot < 32; slot++)
    {
      for (int func = 0; func < 8; func++)
      {
        pci_device_vendor_t pci_dv = pci_get_device_vendor(bus, slot, func);

        if (pci_dv.vendor_id == PCI_DEVICE_VENDOR_NONE)
          continue;

        uart_printf(uart_port1, "[debug] [pci] Bus %d, Slot %d: Vendor: %s (%x), Device: %s (%x), Func: %d\n", 
                    bus, slot, pci_dv.vendor_name, pci_dv.vendor_id,
                    pci_dv.device_name, pci_dv.device_id, func);
      }
    }
  }
}

void debug_sleep(void)
{
  uart_printf(uart_port1, "[debug] Sleeping 1...\n");
  sleep(1);
  uart_printf(uart_port1, "[debug] Sleeping 2...\n");
  sleep(1);
  uart_printf(uart_port1, "[debug] Sleeping 3...\n");
  sleep_ms(1000);
}

void debug_print_true_rand(void)
{
  u32_t rand = true_rand();
  uart_printf(uart_port1, "[debug] true random number: %d\n", rand);
}

void debug_test_task_a_fn(void)
{
  while(1)
  {
    uart_printf(uart_port1, "[debug] Hello from task A!\n");
    sleep_ms(500);
  }
}

void debug_test_task_b_fn(void)
{
  while(1)
  {
    uart_printf(uart_port1, "[debug] Hello from task B!\n");
    sleep_ms(500);
  }
}
