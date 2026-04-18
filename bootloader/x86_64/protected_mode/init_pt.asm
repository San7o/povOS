  ;; -----------------------------------------------------------------
  ;; 
  ;; Initialize the page table
  ;; =========================
  ;;
  ;; We want to map 4MB of virual address from 0x0 and from
  ;; $HIGHER_HALF to the first physical 4MB. To do this, we use 4 page
  ;; entries:
  ;; 
  ;; PML4T     -> 0x1000  Page Map Level 4 Table
  ;; PDPT_LOW  -> 0x2000  Page Directory Pointer Table Low
  ;; PDPT_HIGH -> 0x3000  Page Directory Table High
  ;; PD        -> 0x4000  Page Directory
  ;;
  ;; We set the PML4T[0] to point to PDPT_LOW, and PML4T[511] to point
  ;; to PDPT_HIGH (which corresponds to the range 0xFF8000000000 and
  ;; above). Then we set PDPT_LOW[0] to point to the PD, and
  ;; PDPT_HIGH[510] to point to the same PD (which maps to
  ;; 0xFFFFFFFF80000000). We set the address of PD at 0x0; with huge
  ;; table set, it could address a total of 1GB of memory but we only
  ;; map the first 4MB.
  ;;
  ;; We also setup the recursive mapping trick at
  ;; PML4T[$RECURSIVE_SLOT]. This makes it possible to find the
  ;; physical address of the level 1 page entry itself given a virtual
  ;; address. This is useful for the kernel.
  ;; 

  [bits 32]

init_pt_protected:

  pushad

  ;; Clear the memory area using rep stosd
  ;;
  ;; rep stods is what's known as a "repeating string command",
  ;; meaning that it will write the same thing over and over until a
  ;; certain criteria is met. How does it know when to stop though? We
  ;; tell it with the eax, edi, and ecx registers. eax is the value to
  ;; write, edi is the start address, and ecx is the number of
  ;; repetitions to perform.
  ;;
  ;; Even the instruction has significance. The 'd' at the end of
  ;; 'stosd' tells the cpu to write a "double word" or 4 bytes with
  ;; each repetition (the same size as eax). It also increments edi by
  ;; 4 rather than by 1 to ensure no data overlap.
  
  mov edi, $PML4T_ADDR          ; Set the base address for rep stosd.

  mov ax, data_seg              ; flat data selector
  mov es, ax
  xor eax, eax                  ; Set eax to 0
  mov ecx, 4096                 ; Repeat 4096 times. Since each page
                                ; table is 4096 bytes, and we're
                                ; writing 4 bytes each repetition,
                                ; this will zero out all 5 page
                                ; tables. (4096 / 4 * 4 = 4096)
  rep stosd

  ;; 
  ;; Setup page entries
  ;; 

  ;; PML4T[0]
  mov edi, $PML4T_ADDR
  mov cr3, edi                     ; Register PML4T
  mov dword[edi], $PDPT_LOW_ADDR
  add dword[edi], 0b11             ; Flags: Present + Writable
  mov dword[edi + 4], 0            ; Zero the upper 32 bits
  ;; Do the recursive mapping trick
  ;; Set PML4T[$RECURSIVE_SLOT] to point to PML4T itself
  mov dword [edi + $RECURSIVE_SLOT * 8], $PML4T_ADDR
  add dword [edi + $RECURSIVE_SLOT * 8], 0b11   ; Flags: Present + Writable
  mov dword [edi + $RECURSIVE_SLOT * 8 + 4], 0  ; Upper 32 bits

  ;; PML4T[511]
  mov dword[edi + 4088], $PDPT_HIGH_ADDR
  add dword[edi + 4088], 0b11       ; Flags: Present + Writable
  mov dword[edi + 4092], 0          ; Zero the upper 32 bits

  ;; PDPT-Low[0]
  mov edi, $PDPT_LOW_ADDR
  mov dword[edi], $PDT_ADDR         ; Set addr
  add dword[edi], 0b11              ; Flags: Present + Writable
  mov dword[edi + 4], 0             ; Zero the upper 32 bits

  ;; PDPT-High[510]
  mov edi, $PDPT_HIGH_ADDR
  mov dword[edi + 4080], $PDT_ADDR  ; Set addr
  add dword[edi + 4080], 0b11       ; Flags: Present + Writable
  mov dword[edi + 4084], 0          ; Zero the upper 32 bits

  ;; PDT
  mov edi, $PDT_ADDR
  mov dword[edi], 0x00000000        ; Address
  add dword[edi], 0b10000011        ; Flags: Present + Writable + Huge
  mov dword[edi + 4], 0             ; Zero the upper 32 bits
  mov dword[edi + 8], 0x00200000    ; Address
  add dword[edi + 8], 0b10000011    ; Flags: Present + Writable + Huge
  mov dword[edi + 12], 0            ; Zero the upper 32 bits

  ;; Set up PAE (Physical Address Extension) paging, but don't enable
  ;; it quite yet. PAE defines a page table hierarchy of three levels
  ;; (instead of two), with table entries of 64 bits each instead of
  ;; 32, allowing these CPUs to directly access a physical address
  ;; space larger than 4 gigabytes (232 bytes).
  ;;
  ;; Here we're basically telling the CPU that we want to use paging,
  ;; but not quite yet. We're enabling the feature, but not using it.
  mov eax, cr4
  or eax, 1 << 5                ; Set the PAE-bit, which is the 5th bit
  mov cr4, eax

  ;; Now we should have a page table that identities maps the lowest
  ;; 4MB of physical memory into virtual memory
  popad
  ret
