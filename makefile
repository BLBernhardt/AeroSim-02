#//345678901234567890123456789012345678901234567890123456789012345678901234567890
#//==============================================================================
#//
#//       MAKE file,   AeroSim
#//
#//==============================================================================


PROG   = a
CC     = g++
CFLAGS = -Wall -Wl,-rpath-link 
LIBS   = -lglut -lGLU -lGL -lm

OBJS   =	0-Math/Vector.o \
			0-Math/Matrix.o \
			0-Math/Quaternion.o \
            0-AeroSim/flightmodel-810-03.o \
			0-Graphics/axis.o \
			0-Graphics/0-PFD/PFD_Exec.o \
			0-Graphics/0-PFD/PFD_Heading.o \
			0-Graphics/0-PFD/PFD_Circle.o \
			0-Graphics/0-PFD/PFD_Bezzel.o \
			0-Graphics/0-PFD/PFD_AltTicker.o \
			0-Graphics/0-PFD/PFD_Attitude.o \
			0-Graphics/0-PFD/PFD_RateOfClimb.o \
			0-Graphics/0-PFD/PFD_AltTape.o \
			0-Graphics/0-PFD/PFD_SpeedTape.o \
			0-Graphics/0-PFD/PFD_SpeedTicker.o \
			0-Graphics/0-PFD/PFD_Display.o \
			0-Graphics/0-PFD/Flight_Stick.o \
			0-Graphics/HSI_Map.o \
			0-Graphics/main_graphics.o \
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
	
	
	
	
