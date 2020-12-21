;; An assembly code for all builtin functions used in Latte
;; This already extern's all the libc functions Latte relies on, so no linking
;; using gcc will be needed
;;
extern _start, printf, puts, exit, scanf, stdin, strlen, getline

SECTION .TEXT
.LC0:
    db "%d",0xA,0x0
.GF0: ; printInt
    mov     rsi, rdi
    xor     rax, rax
    mov     rdi, .LC0
    jmp     printf ; TODO: tailcall impossbile, b/c calling convention differs

.GF1: ; printString
    jmp     puts ; TODO: tailcall impossbile, b/c calling convention differs

.LC1:
    db "runtime error",0xA,0x0
.GF2: ; error
    mov     rdi, .LC1
    sub     rsp, 8
    xor     rax, rax
    call    printf
    mov     rdi, 1
    call    exit ; tailcall

.LC2:
    db "%d",0x0
.GF3: ; readInt
    sub     rsp, 24
    mov     rdi, .LC2
    xor     rax, rax
    lea     rsi, [rsp+12]
    call    scanf
    mov     eax, DWORD [rsp+12]
    add     rsp, 24
    ret

.GF4: ; readString
    push    rbx
    sub     rsp, 16
    mov     QWORD [rsp], 0
    mov     QWORD [rsp+8], 0
    mov     rdx, QWORD [rel stdin]
    mov     rdi, rsp
    lea     rsi, [rsp+8]
    call    getline
    mov     rbx, QWORD [rsp]
    mov     rdi, rbx
    call    strlen
    cmp     BYTE [rbx+rax-1], 10
    jne     .GF4_L0
    mov     BYTE [rbx+rax-1], 0
.GF4_L0:
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