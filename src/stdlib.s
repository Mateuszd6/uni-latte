;; An assembly code for all builtin functions used in Latte
;; This already extern's all the libc functions Latte relies on, so no linking
;; using gcc will be needed
;;
extern _start, printf, puts, exit, scanf, stdin, strcat, strcmp, strlen, getline, malloc

.BF0: ; strCompare
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
    push    r15
    push    r14
    push    r12
    push    rbx
    mov     r14, rdi
    test    rdi, rdi
    mov     rax, .BS0
    cmove   r14, rax
    mov     r12, rsi
    test    rsi, rsi
    cmove   r12, rax
    mov     rdi, r14
    call    strlen
    mov     r15, rax
    mov     rdi, r12
    call    strlen
    add     eax, r15d
    add     eax, 1
    movsxd  rdi, eax
    call    malloc
    mov     rbx, rax
    mov     BYTE [rax], 0
    mov     rdi, rax
    mov     rsi, r14
    call    strcat
    mov     rdi, rbx
    mov     rsi, r12
    pop     rbx
    pop     r12
    pop     r14
    pop     r15
    pop     rbp
    jmp     strcat ; TODO: remove tailcall

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
