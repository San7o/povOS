  [bits 64]

  global enable_interrupts
  global disable_interrupts

enable_interrupts:
  sti
  ret

disable_interrupts:
  cli
  ret
