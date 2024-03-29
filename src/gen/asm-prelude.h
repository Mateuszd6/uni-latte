#ifndef ASM_PRELUDE_H_
#define ASM_PRELUDE_H_

static char const gen_asm_intro[] = 
    "extern _start, calloc, exit, free, getline, malloc, printf, puts, sscanf, stdin, strcat, strcmp, strlen\n"
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
    "    push    rcx\n"
    "    push    rsi\n"
    "    push    rdi\n"
    "    push    r8\n"
    "    push    r9\n"
    "    push    r10\n"
    "    push    r11\n"
    "    push    rbx\n"
    "    mov     rdi, QWORD [rbp+16]\n"
    "    mov     rsi, QWORD [rbp+24]\n"
    "    mov     rax, QWORD .BS0\n"
    "    test    rdi, rdi\n"
    "    cmove   rdi, rax\n"
    "    test    rsi, rsi\n"
    "    cmove   rsi, rax\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    strcmp\n"
    "    mov     rsp, rbx\n"
    "    test    eax, eax\n"
    "    sete    al\n"
    "    movzx   eax, al\n"
    "    pop     rbx\n"
    "    pop     r11\n"
    "    pop     r10\n"
    "    pop     r9\n"
    "    pop     r8\n"
    "    pop     rdi\n"
    "    pop     rsi\n"
    "    pop     rcx\n"
    "    pop     rbp\n"
    "    ret\n"
    "\n"
    ".BF1: ; strAdd\n"
    "    push    rbp\n"
    "    mov     rbp, rsp\n"
    "    push    rcx\n"
    "    push    rsi\n"
    "    push    rdi\n"
    "    push    r8\n"
    "    push    r9\n"
    "    push    r10\n"
    "    push    r11\n"
    "    push    rbx\n"
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
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    strlen\n"
    "    mov     rsp, rbx\n"
    "    mov     r15, rax\n"
    "    mov     rdi, r13\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    strlen\n"
    "    mov     rsp, rbx\n"
    "    add     eax, r15d\n"
    "    add     eax, 1\n"
    "    movsxd  rdi, eax\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    malloc\n"
    "    mov     rsp, rbx\n"
    "    mov     r12, rax\n"
    "    mov     BYTE [rax], 0\n"
    "    mov     rdi, rax\n"
    "    mov     rsi, r14\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    strcat\n"
    "    mov     rsp, rbx\n"
    "    mov     rdi, r12\n"
    "    mov     rsi, r13\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    strcat\n"
    "    mov     rsp, rbx\n"
    "    pop     r15\n"
    "    pop     r14\n"
    "    pop     r13\n"
    "    pop     r12\n"
    "    pop     rbx\n"
    "    pop     r11\n"
    "    pop     r10\n"
    "    pop     r9\n"
    "    pop     r8\n"
    "    pop     rdi\n"
    "    pop     rsi\n"
    "    pop     rcx\n"
    "    pop     rbp\n"
    "    ret\n"
    "\n"
    ".BF2: ; allocate\n"
    "    push    rbp\n"
    "    mov     rbp, rsp\n"
    "    push    rcx\n"
    "    push    rsi\n"
    "    push    rdi\n"
    "    push    r8\n"
    "    push    r9\n"
    "    push    r10\n"
    "    push    r11\n"
    "    push    r12\n"
    "    push    rbx\n"
    "    mov     rdi, QWORD [rbp+16]\n"
    "    mov     r12, rdi\n"
    "    add     rdi, 1\n"
    "    mov     rsi, 8\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    calloc\n"
    "    mov     rsp, rbx\n"
    "    mov     QWORD [rax], r12\n"
    "    add     rax, 8\n"
    "    pop     rbx\n"
    "    pop     r12\n"
    "    pop     r11\n"
    "    pop     r10\n"
    "    pop     r9\n"
    "    pop     r8\n"
    "    pop     rdi\n"
    "    pop     rsi\n"
    "    pop     rcx\n"
    "    pop     rbp\n"
    "    ret\n"
    "\n"
    ".LC0:\n"
    "    db \"%ld\",0xA,0x0\n"
    ".GF0: ; printInt\n"
    "    push    rbp\n"
    "    mov     rbp, rsp\n"
    "    push    rcx\n"
    "    push    rsi\n"
    "    push    rdi\n"
    "    push    r8\n"
    "    push    r9\n"
    "    push    r10\n"
    "    push    r11\n"
    "    push    rbx\n"
    "    mov     rsi, QWORD [rbp+16]\n"
    "    xor     rax, rax\n"
    "    mov     rdi, .LC0\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    printf\n"
    "    mov     rsp, rbx\n"
    "    pop     rbx\n"
    "    pop     r11\n"
    "    pop     r10\n"
    "    pop     r9\n"
    "    pop     r8\n"
    "    pop     rdi\n"
    "    pop     rsi\n"
    "    pop     rcx\n"
    "    pop     rbp\n"
    "    ret\n"
    "\n"
    ".GF1: ; printString\n"
    "    push    rbp\n"
    "    mov     rbp, rsp\n"
    "    push    rcx\n"
    "    push    rsi\n"
    "    push    rdi\n"
    "    push    r8\n"
    "    push    r9\n"
    "    push    r10\n"
    "    push    r11\n"
    "    push    rbx\n"
    "    mov     rdi, QWORD [rbp+16]\n"
    "    mov     rax, QWORD .BS0\n"
    "    test    rdi, rdi\n"
    "    cmove   rdi, rax\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    puts\n"
    "    mov     rsp, rbx\n"
    "    pop     rbx\n"
    "    pop     r11\n"
    "    pop     r10\n"
    "    pop     r9\n"
    "    pop     r8\n"
    "    pop     rdi\n"
    "    pop     rsi\n"
    "    pop     rcx\n"
    "    pop     rbp\n"
    "    ret\n"
    "\n"
    ".LC1:\n"
    "    db \"runtime error\",0x0\n"
    ".GF2: ; error\n"
    "    mov     rdi, .LC1\n"
    "    and     rsp, ~0xF\n"
    "    call    puts\n"
    "    mov     rdi, 1\n"
    "    call    exit ; tailcall\n"
    "\n"
    ".LC2:\n"
    "    db \"%ld\",0x0\n"
    ".GF3: ; readInt\n"
    "    push    rbp\n"
    "    mov     rbp, rsp\n"
    "    push    rcx\n"
    "    push    rsi\n"
    "    push    rdi\n"
    "    push    r8\n"
    "    push    r9\n"
    "    push    r10\n"
    "    push    r11\n"
    "    push    rbx\n"
    "    sub     rsp, 32\n"
    "    mov     QWORD [rsp + 16], 0\n"
    "    mov     QWORD [rsp + 24], 0\n"
    "    mov     QWORD [rsp + 8], 0\n"
    "    mov     rdx, QWORD [rel stdin]\n"
    "    lea     rdi, [rsp + 16]\n"
    "    lea     rsi, [rsp + 24]\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    getline\n"
    "    mov     rsp, rbx\n"
    "    mov     rdi, QWORD [rsp + 16]\n"
    "    test    rdi, rdi\n"
    "    je      .GF3_L0\n"
    "    lea     rdx, [rsp + 8]\n"
    "    mov     esi, .LC2\n"
    "    xor     eax, eax\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    sscanf\n"
    "    mov     rsp, rbx\n"
    "    mov     rdi, QWORD [rsp + 16]\n"
    "    jmp     .GF3_L1\n"
    ".GF3_L0:\n"
    "    xor     rdi, rdi\n"
    ".GF3_L1:\n"
    "    xor     eax, eax\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    free\n"
    "    mov     rsp, rbx\n"
    "    mov     rax, QWORD [rsp + 8]\n"
    "    add     rsp, 32\n"
    "    pop     rbx\n"
    "    pop     r11\n"
    "    pop     r10\n"
    "    pop     r9\n"
    "    pop     r8\n"
    "    pop     rdi\n"
    "    pop     rsi\n"
    "    pop     rcx\n"
    "    pop     rbp\n"
    "    ret\n"
    "\n"
    ".GF4: ; readString\n"
    "    push    rbp\n"
    "    mov     rbp, rsp\n"
    "    push    rcx\n"
    "    push    rsi\n"
    "    push    rdi\n"
    "    push    r8\n"
    "    push    r9\n"
    "    push    r10\n"
    "    push    r11\n"
    "    push    rbx\n"
    "    push    r12\n"
    "    sub     rsp, 16\n"
    "    mov     QWORD [rsp], 0\n"
    "    mov     QWORD [rsp+8], 0\n"
    "    mov     rdx, QWORD [rel stdin]\n"
    "    mov     rdi, rsp\n"
    "    lea     rsi, [rsp+8]\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    getline\n"
    "    mov     rsp, rbx\n"
    "    mov     r12, QWORD [rsp]\n"
    "    mov     rdi, r12\n"
    "    mov     rbx, rsp\n"
    "    and     rsp, ~0xF\n"
    "    call    strlen\n"
    "    mov     rsp, rbx\n"
    "    cmp     BYTE [r12+rax-1], 10\n"
    "    jne     .GF4_L0\n"
    "    mov     BYTE [r12+rax-1], 0\n"
    ".GF4_L0:\n"
    "    mov     rax, QWORD [rsp]\n"
    "    add     rsp, 16\n"
    "    pop     r12\n"
    "    pop     rbx\n"
    "    pop     r11\n"
    "    pop     r10\n"
    "    pop     r9\n"
    "    pop     r8\n"
    "    pop     rdi\n"
    "    pop     rsi\n"
    "    pop     rcx\n"
    "    pop     rbp\n"
    "    ret\n"
;

#endif // ASM_PRELUDE_H_
