


CC = gcc
CFLAGS = -Wall -Wextra


TARGET= server_com

OBJS =	4-IPC/2-Server/Server_TxRx.o \
		Server_main.o


TARGET: ${OBJS}
	${CC} -o $(TARGET) ${OBJS}  $(CFLAGS)

clean:
	rm -f *.o server_com
	rm 4-IPC/2-Server/  *.o
