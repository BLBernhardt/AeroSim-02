CC=g++

CFLAGS=-Wall -g

LIBS = -lglut -lGL -lGLU

TARGET= a

OBJS =	main.o \
		axis.o \
		Math.o
	
TARGET: ${OBJS}

	${CC} -o $(TARGET) ${LIBS} ${OBJS}
clean:
	rm *.o a
