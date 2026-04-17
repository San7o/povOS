  [bits 64]

  global enable_interrupts
  global disable_interrupts
  global breakpoint
  global paging_load
  global vmm_invalidate_tlb
  global hcf
  global cpu_halt
  global true_rand

enable_interrupts:
  sti
  ret

disable_interrupts:
  cli
  ret

breakpoint:
  int3
  ret

  ;; -----------------------------------------------------------------
  ;; Load the PML4 table, adrress at [rdi] which needs to be page aligned
paging_load:
  mov cr3, rdi
  ret

  ;; -----------------------------------------------------------------
  ;; Invalidates any translation lookaside buffer (TLB) entries
  ;; specified with the source operand in [rdi]
vmm_invalidate_tlb:
  invlpg [rdi]
  ret

  ;; -----------------------------------------------------------------
  ;; Halt and catch fire
hcf:
  .loop:
  hlt
  jmp .loop
  ret                           ; unreachable
  
  ;; -----------------------------------------------------------------
  ;; Halt the CPU until the next external interrupt occurs,
  ;  effectively putting the processor in a low-power state.
cpu_halt:
  hlt
  ret

  ;; -----------------------------------------------------------------
  ;; Returns a true random number in eax
true_rand:
  push rcx
  push rbx
  
  ;; Check if the function is available

  .rdseed_check:
  mov eax, 7     ; set EAX to request function 7
  mov ecx, 0     ; set ECX to request subfunction 0
  cpuid
  shr ebx, 18    ; the result we want is in EBX
  and ebx, 1     ; test for the flag of interest
  cmp ebx, 1
  jne .rdrand_check
  
  .rdseed_impl:

  mov ecx, 100   ; number of retries
  .rdseed_retry:
    rdseed eax
    jc .done      ; carry flag is set on success
    loop .rdseed_retry
  .rdseed_fail:
  jmp .rdrand_check

  .rdrand_check:
  
  mov eax, 1     ; set EAX to request function 1
  mov ecx, 0     ; set ECX to request subfunction 0
  cpuid
  shr ecx, 30    ; the result we want is in ECX
  and ecx, 1     ; test for the flag of interest
  cmp ecx, 1
  jne .fail
  
  .rdrand_impl:
  mov ecx, 100   ; number of retries
  .rdrand_retry:
  rdrand eax
  jc .done      ; carry flag is set on success
  loop .rdrand_retry
  
  .fail:
  mov eax, 0
  
  .done:

  pop rbx
  pop rcx
  ret
