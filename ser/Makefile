GCC = GCC
#CFLAGS = -Wall -g -O
OBJS = main.o ser.o sql.o interactive.o

ser : ${OBJS}
	${CC} $^ -o $@ -lpthread -lsqlite3

main.o : main.c
	${CC} -c $^ -o $@

ser.o : ser.c
	${CC} -lpthread -Wall -c $< -o $@

sql.o : sql.c
	${CC} -lsqlite3 -Wall -c $< -o $@

interactive.o : interactive.c
	${CC} -lsqlite3 -Wall -c $< -o $@

clean :
	rm ser ${OBJS}
