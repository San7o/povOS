  [bits 64]

  global atomic_cmpxchg

  ;; -----------------------------------------------------------------
  ;; Atomic compare and exhange
  ;; 
  ;; Compares memory pointed by [rdi] to [rdx], if they are equal,
  ;; copies [rsi] to [rdi], otherwise returns memory pointed by [rdi]
atomic_cmpxchg:
  mov rax, rdx
  lock cmpxchg [rdi], rsi
  ;; If [rdi] == rax: 
  ;;    ZF is set, [rdi] becomes rsi (b). rax remains cmp_val.
  ;; If [rdi] != rax: 
  ;;    ZF is cleared, rax is loaded with the current value of [rdi].
  ret
