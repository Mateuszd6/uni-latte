CC = clang
CCFLAGS = -std=c99 -O0 -ggdb -DDEBUG
CWARNINGS = -Wall -Wextra -Wshadow -pedantic -Wno-unused-function -Weverything -Wno-padded -Wno-c++-compat -Wno-gnu-empty-struct -Wno-reserved-id-macro -Wno-missing-noreturn
CSANITIZERS = -fsanitize=address,undefined
CPLATFORM = -D_POSIX_C_SOURCE=200809L
CFORMAT = clang-format -style="{BasedOnStyle: mozilla, TabWidth: 4, IndentWidth: 4, BreakBeforeBraces: Allman, ColumnLimit: 80}" -i

BNFC = /home/students/inf/PUBLIC/MRJP/bin/students/bnfc
BNFC_FLAGS = --c

FLEX = flex
FLEX_FLAGS = -Platte

BISON = bison
BISON_FLAGS = -t -platte

OBJS = ./obj/absyn.o ./obj/lexer.o ./obj/parser.o

.PHONY: all clean grammar validate

all: latc

clean:
	@-rm -rf ./outs ./obj ./cov latc

latc: ${OBJS} src/*.c src/*.h
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

    # Remove junk files
	@-rm -f ./src/Printer.c
	@-rm -f ./src/Printer.h
	@-rm -f ./src/Skeleton.c
	@-rm -f ./src/Skeleton.h
	@-rm -f ./src/Test.c
	@-rm -f ./src/*.bak # just.dont.do.it

    # Don't use capital letters in filenames
	@-mv -f ./src/Absyn.h ./src/absyn.h
	@-mv -f ./src/Absyn.c ./src/absyn.c
	@-mv -f ./src/Parser.h ./src/parser.h

    # .. and in includes
	@-sed -i 's/\#include ".*/\L&/' ./src/absyn.h
	@-sed -i 's/\#include ".*/\L&/' ./src/absyn.c
	@-sed -i 's/\#include ".*/\L&/' ./src/parser.h
	@-sed -i 's/\#include ".*/\L&/' ./src/latte.y
	@-sed -i 's/\#include ".*/\L&/' ./src/latte.l

    # Replace very poor error handling in the bison file to match the
    # requirements in the assignment
	@-sed -i 's/int yy_mylinenumber;/int yy_mylinenumber;\nextern void error(long line, char* fmt, ...);/g' ./src/latte.y
	@-sed -i 's/fprintf(stderr,"error: line %d: %s at %s\\n",/error(yy_mylinenumber + 1, "%s at %s",/g' ./src/latte.y
	@-sed -i 's/yy_mylinenumber + 1, str, lattetext/str, lattetext/g' ./src/latte.y

    # Add include of our misc definitions (for line numbers)
	@-sed -i '4i#include "misc.h"' ./src/absyn.c
	@-sed -i '/fprintf/d; /exit/d' ./src/absyn.c
	@-sed -n -i '1h;1!H;$${;g;s/[ ]*if (!tmp)\n[ ]*{\n[ ]*}\n//g;p;}' ./src/absyn.c

    # This replaces the default node allocation function with the one that saved line numbers
	@-sed -i 's/malloc/alloc_ast_node/g' ./src/absyn.c

    # Format source files. Unfortunetly clang-format cannot format .y (bison) files...
	${CFORMAT} ./src/parser.h
	${CFORMAT} ./src/absyn.c
	${CFORMAT} ./src/absyn.h

validate: latc
	@-./validate.sh 2> /dev/null
