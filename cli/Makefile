GCC = gcc 
CFALGS = -Wall -g -O
OBJS = main.o cli.o interactive.o

cli : ${OBJS}
	${CC} $^ -o $@

main.o : main.c

cli.o : cli.c

interactive.o : interactive.c

clean :
	rm cli ${OBJS}
