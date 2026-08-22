


CC = gcc
CFLAGS = -Wall -Wextra


TARGET= a

OBJS =	Server_TxRx.o \
		Server_main.o


TARGET: ${OBJS}
	${CC} -o $(TARGET) ${OBJS}  $(CFLAGS)

clean:
	rm -f *.o a
	rm 4-IPC/2-Server/  *.o
