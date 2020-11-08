CC = gcc
CCFLAGS = -g -W -Wall

FLEX = flex
FLEX_OPTS = -Platte

BISON = bison
BISON_OPTS = -t -platte

OBJS = absyn.o lexer.o parser.o printer.o

.PHONY: all clean 

all: testlatte

clean:
	@-rm -f *.o main 

testlatte: ${OBJS} test.o
	${CC} ${CCFLAGS} ${OBJS} test.o -o main

absyn.o: absyn.c absyn.h
	${CC} ${CCFLAGS} -c absyn.c

lexer.c: latte.l
	${FLEX} ${FLEX_OPTS} -olexer.c latte.l

parser.c: latte.y
	${BISON} ${BISON_OPTS} latte.y -o parser.c

lexer.o: lexer.c parser.h
	${CC} ${CCFLAGS} -c lexer.c 

parser.o: parser.c absyn.h
	${CC} ${CCFLAGS} -c parser.c

printer.o: printer.c printer.h absyn.h
	${CC} ${CCFLAGS} -c printer.c

test.o: test.c parser.h printer.h absyn.h
	${CC} ${CCFLAGS} -c test.c
