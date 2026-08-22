CC = gcc
CFLAGS = -Wall -Wextra

TARGET= client_com

OBJS =	Client_TxRx.o \
		Client_main.o

TARGET: ${OBJS}
	${CC} -o $(TARGET) ${OBJS}

clean:
	rm *.o  client_com
