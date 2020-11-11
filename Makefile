CC = clang
CCFLAGS = -std=c99 -O0 -ggdb
CWARNINGS = -Wall -Wextra -Wshadow -Wno-unused-function  -Weverything -Wno-padded -Wno-c++-compat -Wno-gnu-empty-struct -Wno-reserved-id-macro
CSANITIZERS = -fsanitize=address,undefined
CPLATFORM = -D_POSIX_C_SOURCE=200809L

FLEX = flex
FLEX_FLAGS = -Platte

BISON = bison
BISON_FLAGS = -t -platte

OBJS = ./obj/main.o ./obj/absyn.o ./obj/lexer.o ./obj/parser.o ./obj/printer.o

.PHONY: all clean validate

all: latc

clean:
	@-rm -f ./obj/*.o latc main

latc: ${OBJS}
	${CC} ${CSANITIZERS} ${CCFLAGS} ${OBJS} -o latc

obj/%.o: src/%.c
	@-mkdir -p obj
	${CC} -c ${CCFLAGS} ${CWARNINGS} ${CSANITIZERS} ${CPLATFORM} $< -o $@

lexer.c: latte.l
	${FLEX} ${FLEX_FLAGS} -olexer.c latte.l

parser.c: latte.y
	${BISON} ${BISON_FLAGS} latte.y -o parser.c

validate: latc
	./validate.sh 2> /dev/null
