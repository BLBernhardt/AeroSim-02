


CC = gcc
CFLAGS = -Wall -Wextra


TARGET= server_com

OBJS =	Server_TxRx.o \
		Server_main.o


TARGET: ${OBJS}
	${CC} -o $(TARGET) ${OBJS}  $(CFLAGS)

clean:
	rm -f *.o server_com
