;; An assembly code for all builtin functions used in Latte
;; This already extern's all the libc functions Latte relies on, so no linking
;; using gcc will be needed. Writing it in C is not possible, because our
;; language does not use cdecl ABI so these would be incompatible.
;;
extern _start, calloc, exit, fgetc, free, getline, malloc, printf, puts, scanf, sscanf, stdin, strcat, strcmp, strlen

;;
