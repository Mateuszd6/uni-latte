#ifndef ASM_PRELUDE_H_
#define ASM_PRELUDE_H_

static char const gen_asm_intro[] = 
    "extern _start, printf, puts, exit, fgetc, free, getline, malloc, scanf, sscanf, stdin, strcat, strcmp, strlen\n"
    "\n"
;

static char const gen_asm_epilogue[] = 
    "\n"
    ";\n"
    "; Builtin functions, handcoded in assembly:\n"
    ";\n"
    "\n"
    ".BF0: ; strCmp\n"
    "    push    rbp\n"
    "    mov     rbp, rsp\n"
    "    mov     rdi, QWORD [rbp+16]\n"
    "    mov     rsi, QWORD [rbp+24]\n"
    "    mov     rax, QWORD .BS0\n"
    "    test    rdi, rdi\n"
    "    cmove   rdi, rax\n"
    "    test    rsi, rsi\n"
    "    cmove   rsi, rax\n"
    "    call    strcmp\n"
    "    test    eax, eax\n"
    "    sete    al\n"
    "    movzx   eax, al\n"
    "    pop     rbp\n"
    "    ret\n"
    "\n"
    ".BF1: ; strAdd\n"
    "    push    rbp\n"
    "    mov     rbp, rsp\n"
    "    push    r12\n"
    "    push    r13\n"
    "    push    r14\n"
    "    push    r15\n"
    "    mov     rdi, QWORD [rbp+16]\n"
    "    mov     rsi, QWORD [rbp+24]\n"
    "    mov     rax, QWORD .BS0\n"
    "    mov     r14, rdi\n"
    "    test    rdi, rdi\n"
    "    cmove   r14, rax\n"
    "    mov     r13, rsi\n"
    "    test    rsi, rsi\n"
    "    cmove   r13, rax\n"
    "    mov     rdi, r14\n"
    "    call    strlen\n"
    "    mov     r15, rax\n"
    "    mov     rdi, r13\n"
    "    call    strlen\n"
    "    add     eax, r15d\n"
    "    add     eax, 1\n"
    "    movsxd  rdi, eax\n"
    "    call    malloc\n"
    "    mov     r12, rax\n"
    "    mov     BYTE [rax], 0\n"
    "    mov     rdi, rax\n"
    "    mov     rsi, r14\n"
    "    call    strcat\n"
    "    mov     rdi, r12\n"
    "    mov     rsi, r13\n"
    "    call    strcat\n"
    "    pop     r15\n"
    "    pop     r14\n"
    "    pop     r13\n"
    "    pop     r12\n"
    "    pop     rbp\n"
    "    ret\n"
    "\n"
    ".LC0:\n"
    "    db \"%d\",0xA,0x0\n"
    ".GF0: ; printInt\n"
    "    push    rbp\n"
    "    mov     rbp, rsp\n"
    "    mov     rsi, QWORD [rbp+16]\n"
    "    xor     rax, rax\n"
    "    mov     rdi, .LC0\n"
    "    mov     r13, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    printf\n"
    "    mov     rsp, r13\n"
    "    pop     rbp\n"
    "    ret\n"
    "\n"
    ".GF1: ; printString\n"
    "    push    rbp\n"
    "    mov     rbp, rsp\n"
    "    mov     rdi, QWORD [rbp+16]\n"
    "    mov     rax, QWORD .BS0\n"
    "    test    rdi, rdi\n"
    "    cmove   rdi, rax\n"
    "    call    puts\n"
    "    pop     rbp\n"
    "    ret\n"
    "\n"
    ".LC1:\n"
    "    db \"runtime error\",0x0\n"
    ".GF2: ; error\n"
    "    mov     rdi, .LC1\n"
    "    call    puts\n"
    "    mov     rdi, 1\n"
    "    call    exit ; tailcall\n"
    "\n"
    ".LC2:\n"
    "    db \"%d\",0x0\n"
    ".GF3: ; readInt\n"
    "    push    rbp\n"
    "    push    r13\n"
    "    mov     rbp, rsp\n"
    "    sub     rsp, 32\n"
    "    mov     QWORD [rbp - 16], 0\n"
    "    mov     QWORD [rbp - 24], 0\n"
    "    mov     DWORD [rbp - 4], 0\n"
    "    mov     rdx, QWORD [rel stdin]\n"
    "    lea     rdi, [rbp - 16]\n"
    "    lea     rsi, [rbp - 24]\n"
    "    call    getline\n"
    "    mov     rdi, QWORD [rbp - 16]\n"
    "    test    rdi, rdi\n"
    "    je      .GF3_L0\n"
    "    lea     rdx, [rbp - 4]\n"
    "    mov     esi, .LC2\n"
    "    xor     eax, eax\n"
    "    mov     r13, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    sscanf\n"
    "    mov     rsp, r13\n"
    "    mov     rdi, QWORD [rbp - 16]\n"
    "    jmp     .GF3_L1\n"
    ".GF3_L0:\n"
    "    xor     edi, edi\n"
    ".GF3_L1:\n"
    "    xor     eax, eax\n"
    "    call    free\n"
    "    mov     eax, DWORD [rbp - 4]\n"
    "    pop     r13\n"
    "    add     rsp, 32\n"
    "    pop     rbp\n"
    "    ret\n"
    "\n"
    ".GF4: ; readString\n"
    "    push    rbx\n"
    "    sub     rsp, 16\n"
    "    mov     QWORD [rsp], 0\n"
    "    mov     QWORD [rsp+8], 0\n"
    "    mov     rdx, QWORD [rel stdin]\n"
    "    mov     rdi, rsp\n"
    "    lea     rsi, [rsp+8]\n"
    "    call    getline\n"
    "    mov     rbx, QWORD [rsp]\n"
    "    mov     rdi, rbx\n"
    "    call    strlen\n"
    "    cmp     BYTE [rbx+rax-1], 10\n"
    "    jne     .GF4_L0\n"
    "    mov     BYTE [rbx+rax-1], 0\n"
    ".GF4_L0:\n"
    "    mov     rax, QWORD [rsp]\n"
    "    add     rsp, 16\n"
    "    pop     rbx\n"
    "    ret\n"
;

#endif // ASM_PRELUDE_H_
