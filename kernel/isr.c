// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/isr.h>   // implements
#include <kernel/debug.h>
#include <kernel/time.h>
#include <kernel/macros.h>
#include <kernel/sched.h>
#include <drivers/isa/uart.h>
#include <drivers/isa/ps2.h>
#include <drivers/isa/pic.h>
#include <drivers/input/keyboard.h>

#include "isr_string.h"

void isr_common_handler(u8_t  isr_number,
                        u64_t error_code,
                        u64_t* sp)
{
  uart_write_str(uart_port1, "[isr] ");
  if (isr_number < ISR_EXCEPTION_COUNT)
    uart_write_str(uart_port1, isr_exception_string[isr_number]);
  else
    uart_write_hex(uart_port1, isr_number);
  uart_write_str(uart_port1, ", error code: ");
  uart_write_hex(uart_port1, error_code);
  uart_putc(uart_port1, '\n');

  // Dump registers
  cpu_regs_t ct_regs;
  ct_regs = scheduler.tasks[current_task].task.regs;
  // These need to match the assembly code
  ct_regs.rax    = sp[0];
  ct_regs.rcx    = sp[1];
  ct_regs.rdx    = sp[2];
  ct_regs.rbx    = sp[3];
  ct_regs.rbp    = sp[4];
  ct_regs.rsi    = sp[5];
  ct_regs.rdi    = sp[6];
  ct_regs.r8     = sp[7];
  ct_regs.r9     = sp[8];
  ct_regs.r10    = sp[9];
  ct_regs.r11    = sp[10];
  ct_regs.r12    = sp[11];
  ct_regs.r13    = sp[12];
  ct_regs.r14    = sp[13];
  ct_regs.r15    = sp[14];
  ct_regs.rip    = sp[17];
  ct_regs.rflags = sp[19];
  ct_regs.rsp    = sp[20];
  ct_regs.cr3    = regs_get_cr3();
  debug_dump_regs_uart3(ct_regs);
  
  pic_ack();
  return;
}

void isr_keyboard_handler(u8_t  isr_number,
                          u64_t error_code,
                          u64_t* sp)
{
  UNUSED(isr_number);
  UNUSED(error_code);
  UNUSED(sp);

  keyboard_t *keyboard;
  keyboard_event_t event;
  u8_t scancode;
  
  keyboard = keyboard_get_active();
  if (!keyboard) goto exit;
  
  scancode = ps2_read_data();
  event =
    keyboard_event_from_scancode(keyboard, scancode);
  if (event.key == KEY_NONE) goto exit;

  // Update the keyboard
  keyboard_update(keyboard, event);
  
  // Debug print via UART
  // debug_dump_keyboard_event_uart(event);

 exit:
  pic_ack();
  return;
}

void isr_pit_channel_0_handler(u8_t  isr_number,
                               u64_t error_code,
                               u64_t* sp)
{
  UNUSED(isr_number);
  UNUSED(error_code);
  UNUSED(sp);
  
  cpu_regs_t* ct_regs;
  
  time_ms++;
  pic_ack();

  if (!scheduler.initialized)
    return;

  static u64_t last_sched_time_ms = 0;
  if (time_ms - last_sched_time_ms > SCHED_FREQ * 1000)
  {
    // Call the scheduler
    
    // Save registers of the current task
    ct_regs = &scheduler.tasks[current_task].task.regs;
    // These need to match the assembly code
    ct_regs->rax    = sp[0];
    ct_regs->rcx    = sp[1];
    ct_regs->rdx    = sp[2];
    ct_regs->rbx    = sp[3];
    ct_regs->rbp    = sp[4];
    ct_regs->rsi    = sp[5];
    ct_regs->rdi    = sp[6];
    ct_regs->r8     = sp[7];
    ct_regs->r9     = sp[8];
    ct_regs->r10    = sp[9];
    ct_regs->r11    = sp[10];
    ct_regs->r12    = sp[11];
    ct_regs->r13    = sp[12];
    ct_regs->r14    = sp[13];
    ct_regs->r15    = sp[14];
    ct_regs->rip    = sp[17];
    ct_regs->rflags = sp[19];
    ct_regs->rsp    = sp[20];
    ct_regs->cr3     = regs_get_cr3();
    
    last_sched_time_ms = time_ms;
    
    sched_switch_next();

    // Unreachable
  }
  
  return;
}
