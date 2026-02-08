  [bits 64]

  global enable_interrupts
  global disable_interrupts
  global breakpoint

enable_interrupts:
  sti
  ret

disable_interrupts:
  cli
  ret

breakpoint:
  int3
  ret
