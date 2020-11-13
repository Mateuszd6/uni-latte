CC = clang
CCFLAGS = -std=c99 -O0 -ggdb
CWARNINGS = -Wall -Wextra -Wshadow -Wno-unused-function -Weverything -Wno-padded -Wno-c++-compat -Wno-gnu-empty-struct -Wno-reserved-id-macro -Wno-missing-noreturn
CSANITIZERS = -fsanitize=address,undefined
CPLATFORM = -D_POSIX_C_SOURCE=200809L
CFORMAT = clang-format -style="{BasedOnStyle: mozilla, TabWidth: 4, IndentWidth: 4, BreakBeforeBraces: Allman, ColumnLimit: 80}" -i

BNFC = /home/students/inf/PUBLIC/MRJP/bin/students/bnfc
BNFC_FLAGS = --c

FLEX = flex
FLEX_FLAGS = -Platte

BISON = bison
BISON_FLAGS = -t -platte

OBJS = ./obj/absyn.o ./obj/lexer.o ./obj/parser.o ./obj/printer.o # TODO: Kill printer

.PHONY: all clean grammar validate

all: latc

clean:
	@-rm -rf ./outs ./obj latc

latc: ${OBJS} src/*.c
	${CC} ${CCFLAGS} ${CWARNINGS} ${CSANITIZERS} ${CPLATFORM} ${OBJS} ./src/main.c -o latc

obj/%.o: src/%.c
	@-mkdir -p obj
	${CC} -c ${CCFLAGS} ${CWARNINGS} ${CSANITIZERS} ${CPLATFORM} $< -o $@

src/lexer.c: src/latte.l
	${FLEX} ${FLEX_FLAGS} -osrc/lexer.c src/latte.l
	${CFORMAT} ./src/lexer.c

src/parser.c: src/latte.y
	${BISON} ${BISON_FLAGS} src/latte.y -o src/parser.c
	${CFORMAT} ./src/parser.c

grammar:
	${BNFC} ${BNFC_FLAGS} -o ./src/ ./src/latte.cf
	@-mv -f ./src/Absyn.h ./src/absyn.h
	@-mv -f ./src/Absyn.c ./src/absyn.c
	@-mv -f ./src/Parser.h ./src/parser.h
	@-mv -f ./src/Printer.c ./src/printer.c
	@-mv -f ./src/Printer.h ./src/printer.h
	@-rm -f ./src/Skeleton.c
	@-rm -f ./src/Skeleton.h
	@-rm -f ./src/Test.c
	@-rm -f ./src/*.bak # just.dont.do.it
	@-sed -i 's/\#include ".*/\L&/' ./src/absyn.h
	@-sed -i 's/\#include ".*/\L&/' ./src/absyn.c
	@-sed -i 's/\#include ".*/\L&/' ./src/printer.h
	@-sed -i 's/\#include ".*/\L&/' ./src/printer.c
	@-sed -i 's/\#include ".*/\L&/' ./src/parser.h
	@-sed -i 's/\#include ".*/\L&/' ./src/latte.y
	@-sed -i 's/\#include ".*/\L&/' ./src/latte.l
	@-sed -i 's/fprintf(stderr,"error: line %d: %s at %s\\n",/fprintf(stderr,"ERROR\\n%s:%d: error: %s at %s\\n", "TODO.lat",/g' ./src/latte.y
	@-sed -i '4i#include "misc.h"' ./src/absyn.c
	@-sed -i '/fprintf/d; /exit/d' ./src/absyn.c
	@-sed -n -i '1h;1!H;$${;g;s/[ ]*if (!tmp)\n[ ]*{\n[ ]*}\n//g;p;}' ./src/absyn.c
	@-sed -i 's/malloc/alloc_ast_node/g' ./src/absyn.c

	${CFORMAT} ./src/parser.h
	${CFORMAT} ./src/absyn.c
	${CFORMAT} ./src/absyn.h
	${CFORMAT} ./src/printer.h
	${CFORMAT} ./src/printer.c

validate: latc
	@-./validate.sh 2> /dev/null
