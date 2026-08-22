CC = gcc
CFLAGS = -Wall -Wextra

TARGET= a

OBJS =	Client_TxRx.o \
		Client_main.o

TARGET: ${OBJS}
	${CC} -o $(TARGET) ${OBJS}

clean:
	rm *.o  a
