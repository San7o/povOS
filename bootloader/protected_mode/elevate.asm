  [bits 32]

  ;; -----------------------------------------------------------------
  ;; Elevate to 64-bit mode
elevate_protected:
  cli
  
  lgdt [gdt_64_descriptor]

  mov eax, cr4
  or eax, 1 << 5                ; CR4.PAE (Physical Address Extension)
  mov cr4, eax
  
  mov ecx, 0xC0000080
  rdmsr
  or eax, 1 << 8
  wrmsr

  ;; Enable paging
  mov eax, cr0
  or eax, 1 << 31               ; CR0.PG (paging)
  mov cr0, eax

  jmp code_seg_64:init_lm

  [bits 64]
  ;; -----------------------------------------------------------------
  ;; Initialize long mode
init_lm:
  cli
  mov ax, data_seg_64           ; Set the A-register to the data descriptor
  mov ds, ax                    ; Set the data segment to the A-register
  mov es, ax                    ; Set the extra segment to the A-register
  mov fs, ax                    ; Set the F-segment to the A-register
  mov gs, ax                    ; Set the G-segment to the A-register
  mov ss, ax                    ; Set the stack segment to the A-register

  jmp begin_long_mode
