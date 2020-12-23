;; An assembly code for all builtin functions used in Latte
;; This already extern's all the libc functions Latte relies on, so no linking
;; using gcc will be needed. Writing it in C is not possible, because our
;; language does not use cdecl ABI so these would be incompatible.
;;
extern _start, printf, puts, exit, scanf, stdin, strcat, strcmp, strlen, getline, malloc

.BF0: ; strCmp
    push    rbp
    mov     rbp, rsp
    ;; First param in our ABI
    mov     rdi, QWORD [rbp+16]
    ;; Second param in our ABI
    mov     rsi, QWORD [rbp+24]
    ;; Null string is the same as "", so nullcheck, BS0 is an empty string
    mov     rax, QWORD .BS0
    test    rdi, rdi
    cmove   rdi, rax
    ;; Same for second string
    test    rsi, rsi
    cmove   rsi, rax
    ;; Now compare two strings
    call    strcmp
    ;; Return 1 if strcmp returned 0, which means equal
    test    eax, eax
    sete    al
    movzx   eax, al
    pop     rbp
    ret

.BF1: ; strAdd
    push    rbp
    mov     rbp, rsp
    push    r12
    push    r13
    push    r14
    push    r15
    ;; First param in our ABI
    mov     rdi, QWORD [rbp+16]
    ;; Second param in our ABI
    mov     rsi, QWORD [rbp+24]
    mov     rax, QWORD .BS0
    ;; Null string is the same as "", so nullcheck, BS0 is an empty string
    mov     r14, rdi
    test    rdi, rdi
    cmove   r14, rax
    ;; Same for second string
    mov     r13, rsi
    test    rsi, rsi
    cmove   r13, rax
    ;; r14 <- first string
    ;; r13 <- second string
    mov     rdi, r14
    call    strlen
    ;; r15 <- len of first string
    mov     r15, rax
    mov     rdi, r13
    call    strlen
    add     eax, r15d
    add     eax, 1
    ;; rax <- len of both strings + 1 (size of allocation)
    movsxd  rdi, eax
    call    malloc
    ;; r12 <- allocated memory
    mov     r12, rax
    ;; Write 0 to the first byte of the buffer, so that we can strcat them
    mov     BYTE [rax], 0
    mov     rdi, rax
    mov     rsi, r14
    ;; Concat first string
    call    strcat
    mov     rdi, r12
    mov     rsi, r13
    ;; Concat second string
    call    strcat
    ;; strcat returns a pointer to the resulting string, so we are ok here
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbp
    ret

.LC0:
    db "%d",0xA,0x0
.GF0: ; printInt
    push    rbp
    mov     rbp, rsp
    ;; First param in our ABI
    mov     rsi, QWORD [rbp+16]
    ;; Variadic functions have number of sse args (0 here) in AX register
    xor     rax, rax
    mov     rdi, .LC0
    call    printf
    pop     rbp
    ret

.GF1: ; printString
    push    rbp
    mov     rbp, rsp
    ;; First param in our ABI
    mov     rdi, QWORD [rbp+16]
    ;; Null string is the same as "", so nullcheck, BS0 is an empty string
    mov     rax, QWORD .BS0
    test    rdi, rdi
    cmove   rdi, rax
    ;;
    call    puts
    pop     rbp
    ret

.LC1:
    db "runtime error",0x0
.GF2: ; error
    mov     rdi, .LC1
    call    puts
    mov     rdi, 1
    call    exit ; tailcall

.LC2:
    db "%d",0x0
.GF3: ; readInt
    push    rbp
    mov     rbp, rsp
    sub     rsp, 24
    mov     rdi, .LC2
    ;; Variadic functions have number of sse args (0 here) in AX register
    xor     rax, rax
    ;; Address of a stack-allocated integer, that gets scanf'ed
    lea     rsi, [rsp+12]
    call    scanf
    ;; ... and then returned
    mov     eax, DWORD [rsp+12]
    add     rsp, 24
    pop     rbp
    ret

.GF4: ; readString
    push    rbx
    sub     rsp, 16
    ;; [rsp+0] is char** lineptr, it will get malloc'ed by getline
    mov     QWORD [rsp], 0
    ;; [rsp+8] is size_t* n, will be the size of a allocation
    mov     QWORD [rsp+8], 0
    ;; Get a global stdin symbol (this is a NASM sytax for RIP relative constant)
    mov     rdx, QWORD [rel stdin]
    mov     rdi, rsp
    lea     rsi, [rsp+8]
    call    getline
    ;; Now we need to remove a trailing \n, because getline keeps adding
    mov     rbx, QWORD [rsp]
    ;; First compute the length of the string, because [rsp+8] holds the size of
    ;; the _allocation_ which is not the same
    mov     rdi, rbx
    call    strlen
    ;; Now check if one-before-last character is '\n'
    cmp     BYTE [rbx+rax-1], 10
    jne     .GF4_L0
    ;; ... and if so, remove it
    mov     BYTE [rbx+rax-1], 0
.GF4_L0:
    ;; The pointer to the allocated string gets returned
    mov     rax, QWORD [rsp]
    add     rsp, 16
    pop     rbx
    ret

;;
;; In order to test one can uncomment _start func and
;;
;; nasm -f elf64 -F dwarf -g fooz.s -o fooz.o
;; ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o fooz -lc fooz.o
;;
;;
;; _start:
;;     call    .GF4
;;     mov     rdi, rax
;;     call    .GF1
;;     jmp     .GF2
