
;
; Builtin functions, handcoded in assembly:
;

.BF0: ; strCmp
    push    rbp
    mov     rbp, rsp
    ;; Save all registers
    push    rcx
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15
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
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    strcmp
    mov     rsp, rbx
    ;; Return 1 if strcmp returned 0, which means equal
    test    eax, eax
    sete    al
    movzx   eax, al
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rcx
    pop     rbp
    ret

.BF1: ; strAdd
    push    rbp
    mov     rbp, rsp
    ;; Save all registers
    push    rcx
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11
    push    rbx
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
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    strlen
    mov     rsp, rbx
    ;; r15 <- len of first string
    mov     r15, rax
    mov     rdi, r13
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    strlen
    mov     rsp, rbx
    add     eax, r15d
    add     eax, 1
    ;; rax <- len of both strings + 1 (size of allocation)
    movsxd  rdi, eax
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    malloc
    mov     rsp, rbx
    ;; r12 <- allocated memory
    mov     r12, rax
    ;; Write 0 to the first byte of the buffer, so that we can strcat them
    mov     BYTE [rax], 0
    mov     rdi, rax
    mov     rsi, r14
    ;; Concat first string
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    strcat
    mov     rsp, rbx
    mov     rdi, r12
    mov     rsi, r13
    ;; Concat second string
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    strcat
    mov     rsp, rbx
    ;; strcat returns a pointer to the resulting string, so we are ok here
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rcx
    pop     rbp
    ret

.LC0:
    db "%d",0xA,0x0
.GF0: ; printInt
    push    rbp
    mov     rbp, rsp
    ;; Save all registers
    push    rcx
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15
    ;; First param in our ABI
    mov     rsi, QWORD [rbp+16]
    ;; Variadic functions have number of sse args (0 here) in AX register
    xor     rax, rax
    mov     rdi, .LC0
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    printf
    mov     rsp, rbx
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rcx
    pop     rbp
    ret

.GF1: ; printString
    push    rbp
    mov     rbp, rsp
    ;; Save all registers
    push    rcx
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15
    ;; First param in our ABI
    mov     rdi, QWORD [rbp+16]
    ;; Null string is the same as "", so nullcheck, BS0 is an empty string
    mov     rax, QWORD .BS0
    test    rdi, rdi
    cmove   rdi, rax
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    puts
    mov     rsp, rbx
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rcx
    pop     rbp
    ret

.LC1:
    db "runtime error",0x0
.GF2: ; error
    ;; No need to save, because func is _Noreturn
    mov     rdi, .LC1
    and     rsp, ~0xF
    call    puts
    mov     rdi, 1
    call    exit ; tailcall

.LC2:
    db "%d",0x0
.GF3: ; readInt
    push    rbp
    mov     rbp, rsp
    ;; Save all registers
    push    rcx
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15
    sub     rsp, 32
    ;; Two params of getline we need to give address to (ptr + length)
    mov     QWORD [rbp - 16], 0
    mov     QWORD [rbp - 24], 0
    ;; Address of a stack-allocated integer, that gets scanf'ed
    mov     DWORD [rbp - 4], 0
    ;; Getline params, stdin + 2 stack variables
    mov     rdx, QWORD [rel stdin]
    lea     rdi, [rbp - 16]
    lea     rsi, [rbp - 24]
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    getline
    mov     rsp, rbx
    ;; If getline failed, we will simply return 0
    mov     rdi, QWORD [rbp - 16]
    test    rdi, rdi
    je      .GF3_L0
    ;; On success, we scanf loaded buffer to get the integer
    lea     rdx, [rbp - 4]
    mov     esi, .LC2
    xor     eax, eax
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    sscanf
    mov     rsp, rbx
    ;; Now move a poiner into a register, because we probably need to free it
    mov     rdi, QWORD [rbp - 16]
    jmp     .GF3_L1
.GF3_L0:
    xor     edi, edi
.GF3_L1:
    ;; Be a good citizen and free mem. Free does nothing on 0, so its fine
    xor     eax, eax
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    free
    mov     rsp, rbx
    ;; Return the integer on the stack, that scanf might have set
    mov     eax, DWORD [rbp - 4]
    add     rsp, 32
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rcx
    pop     rbp
    ret

.GF4: ; readString
    push    rbp
    mov     rbp, rsp
    ;; Save all registers
    push    rcx
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15
    sub     rsp, 16
    ;; [rsp+0] is char** lineptr, it will get malloc'ed by getline
    mov     QWORD [rsp], 0
    ;; [rsp+8] is size_t* n, will be the size of a allocation
    mov     QWORD [rsp+8], 0
    ;; Get a global stdin symbol (this is a NASM sytax for RIP relative constant)
    mov     rdx, QWORD [rel stdin]
    mov     rdi, rsp
    lea     rsi, [rsp+8]
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    getline
    mov     rsp, rbx
    ;; Now we need to remove a trailing \n, because getline keeps adding
    mov     r12, QWORD [rsp]
    ;; First compute the length of the string, because [rsp+8] holds the size of
    ;; the _allocation_ which is not the same
    mov     rdi, r12
    ;; Save stack ptr and align it to 16 byte boundary when calling C api
    mov     rbx, rsp
    and     rsp, ~0xF
    call    strlen
    mov     rsp, rbx
    ;; Now check if one-before-last character is '\n'
    cmp     BYTE [r12+rax-1], 10
    jne     .GF4_L0
    ;; ... and if so, remove it
    mov     BYTE [r12+rax-1], 0
.GF4_L0:
    ;; The pointer to the allocated string gets returned
    mov     rax, QWORD [rsp]
    add     rsp, 16
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rcx
    pop     rbp
    ret
