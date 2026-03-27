  [bits 64]

  global enable_interrupts
  global disable_interrupts
  global breakpoint
  global paging_load
  global vmm_invalidate_tlb

enable_interrupts:
  sti
  ret

disable_interrupts:
  cli
  ret

breakpoint:
  int3
  ret

  ;; Load the PML4 table, adrress at [rdi] which needs to be page aligned
paging_load:
  mov cr3, rdi
  ret

 ;; Invalidates any translation lookaside buffer (TLB) entries
 ;; specified with the source operand in [rdi]
vmm_invalidate_tlb:
  invlpg [rdi]
  ret
