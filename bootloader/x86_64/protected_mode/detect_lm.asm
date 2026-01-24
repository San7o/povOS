  ;; -----------------------------------------------------------------
  ;; 
  ;; Detecting long mode
  ;; ===================
  ;;
  ;; Long mode is supported on all 64-bit CPUs, but not on 32-bit CPUS.
  ;; However, since we've bee in protected mode, we have no idea which
  ;; type of system we're running on. We need to figure out if we can
  ;; even use long mode so that we don't cause a ton of errors when
  ;; trying to activate it.

  [bits 32]

  ;; -----------------------------------------------------------------
  ;; Detect if long mode is available
  ;; Set eax to 1 if available, 0 otherwise
detect_lm_protected:
  pushad

  ;; In order to detect long mode, we need to use the built-in command
  ;; "cpuid". However, CPUID has different modes, and some CPUS don't
  ;; support the additional functionalities to check for long mode,
  ;; even if the support CPUID. So this makes the check for long mode
  ;; way more convoluted that it needs to be. Once again, the steps are:
  ;;
  ;; 1. Check for CPUID
  ;; 2. Check for CPUID extended funcitons
  ;; 3. Check for long mode support

  ;; Checking for CPUID
  ;;
  ;; We can check for the existence of CPUID by using a bit in the
  ;; FLAGS register. This register cannot be accessed by the mov
  ;; command, so we must actually push it onto the stack and pop it
  ;; off to read or write to it.
  ;;
  ;; The bit that tells us if CPUID is supported is bit 21. IF CPUID
  ;; is NOT supported, then bit but MUST take on a certain value.
  ;; However, if CPUID is supported, then it'll take on whatever value
  ;; we give it. We can try this by reading from FLAGS, fliping the
  ;; bit, writing to FLAGS, and the reading again and comparing to the
  ;; earlier read. If ther're equal, the the but is 0

  ;; Read from FLAGS
  pushfd                        ; Copy FLAGS to eax via stack
  pop eax

  ;; Save to ecx for comparison later
  mov ecx, eax

  ;; Flip the ID bit (21st bit of eax)
  xor eax, 1 << 21

  ;; Write to FLAGS
  push eax
  popfd

  ;; Read from FLAGS again
  ;; Bit will be flipped if CPUID supported
  pushfd
  pop eax

  ;; Restore eflags to the older version saved in ecx
  push ecx
  popfd

  ;; Perform the comparison
  ;; If equal, the the bit got flipped back during copy, and CPUID is
  ;; not supported
  cmp eax, ecx
  je .cpuid_not_found           ; Print error and hand if CPUID unsupported

  ;; Check for extended functions of CPUID
  ;;
  ;; Now that we know CPUID exists, we can use it to see whether it
  ;; supports the extended functions needed to enable long mode. The
  ;; CPUID function takes an argument in eax and returns the value in
  ;; eax. To check for extended capabilities, we use the argument
  ;; mov eax, 0x80000000. If extended capabilities exist, then the
  ;; value will be set to grater than 0x80000000, otherwire it will
  ;; stay the same.
  mov eax, 0x80000000           ; CPUID argument 0x80000000
  cpuid                         ; Run the command
  cmp eax, 0x80000001           ; See if result is larget than 0x80000001
  jb .cpuid_not_found           ; If not, error and hang

  ;; Actually check for long mode
  ;;
  ;; Now, we can use CPUID to check for long mode. If long mode is
  ;; supported, then CPUID will set the 29th bit of register edx. The
  ;; eax code to look for long mode is 0x80000001
  mov eax, 0x80000001           ; Set CPUID argument
  cpuid                         ; Run CPUID
  test edx, 1 << 29             ; See if bit 29 set in edx
  jz .lm_not_found              ; If it is not, then error and hang

  ;; Return from the function
  .exit:
  popad
  mov eax, 1                    ; long mode available
  ret
 
  ;; Print and error message and hang
  .cpuid_not_found:
  call clear_protected
  mov esi, cpuid_not_found_str
  call print_protected
  jmp .exit_failure

  ;; Print and error message and hang
  .lm_not_found: 
  call clear_protected
  mov esi, lm_not_found_str
  call print_protected

  .exit_failure:
  popad
  mov eax, 0
  ret
  
lm_not_found_str: db `ERROR: Long mode not supported. Exiting...`, 0
cpuid_not_found_str:  db `ERROR: CPUID unsupported, but required for long mode`, 0
