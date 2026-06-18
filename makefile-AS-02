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

OBJS =		0-AeroModel/coeff_table.o \
			0-AeroModel/aero_model.o \
			main.o 

TARGET= a

${PROG}: ${OBJS}
	${CC} -o ${PROG} ${OBJS} ${LIBS}

clean:
	rm -f *~
	rm -f *.o
	rm -f $(TARGET)
	rm -f a
	rm -f 0-Graphics/*.o
	rm -f 0-Graphics/0-PFD/*.o
	rm -f 0-Math/*.o
	rm -f 0-AeroSim/*.o
	
	
	
	
