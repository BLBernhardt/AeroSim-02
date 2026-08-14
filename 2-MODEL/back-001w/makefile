#//345678901234567890123456789012345678901234567890123456789012345678901234567890
#//==============================================================================
#//
#// MAKE file, AeroSim
#//
#//==============================================================================


PROG = a
CC = g++
CFLAGS = -Wall -Wl,-rpath-link 
LIBS = -lglut -lGLU -lGL -lm

OBJS =		coeff_table.o \
			aero_model.o \
			main.o 

TARGET= a

${PROG}: ${OBJS}
	${CC} -o ${PROG} ${OBJS} ${LIBS}

clean:
	rm -f *~
	rm -f *.o
	rm -f $(TARGET)
	rm -f a

	
	
	
	
