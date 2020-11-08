CC = clang
CCFLAGS = -std=c99 -O0 -ggdb
CWARNINGS = -Wall -Wextra -Wshadow -Wno-unused-function -Weverything -Wno-padded -Wno-c++-compat -Wno-gnu-empty-struct -Wno-reserved-id-macro
CSANITIZERS = -fsanitize=address,undefined
CPLATFORM = -D_POSIX_C_SOURCE=200809L

FLEX = flex
FLEX_OPTS = -Platte

BISON = bison
BISON_OPTS = -t -platte

OBJS = ./obj/absyn.o ./obj/lexer.o ./obj/parser.o ./obj/printer.o

.PHONY: all clean

all: testlatte

clean:
	@-rm -f ./obj/*.o main

testlatte: ${OBJS} ./obj/test.o
	${CC} ${CSANITIZERS} ${CCFLAGS} ${OBJS} ./obj/test.o -o main

obj/%.o: src/%.c
	@-mkdir -p obj
	${CC} -c ${CCFLAGS} ${CWARNINGS} ${CSANITIZERS} ${CPLATFORM} $< -o $@

lexer.c: latte.l
	${FLEX} ${FLEX_OPTS} -olexer.c latte.l

parser.c: latte.y
	${BISON} ${BISON_OPTS} latte.y -o parser.c
