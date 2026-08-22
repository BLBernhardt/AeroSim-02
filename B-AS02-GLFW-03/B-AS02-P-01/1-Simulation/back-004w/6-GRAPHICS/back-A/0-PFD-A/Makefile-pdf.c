


PROG   = a
CC     = gcc
CFLAGS = -Wall -Wl,-rpath-link
LIBS   =-lglut -lGLU -lGL -lm

OBJS   =	PFD_Heading.o \
			PFD_Circle.o \
			PFD_Bezzel.o \
			PFD_AltTicker.o \
			PFD_Attitude.o \
			PFD_AltTape.o \
			PFD_SpeedTape.o \
			PFD_SpeedTicker.o \
			PFD_Display.o \
			flight_dynamics.o \
			main.o


${PROG}: ${OBJS}
	${CC} -o ${PROG} ${OBJS} ${LIBS}

clean:
	rm -rf *.o ${PROG}

