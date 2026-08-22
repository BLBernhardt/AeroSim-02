#//345678901234567890123456789012345678901234567890123456789012345678901234567890
#//==============================================================================
#//
#//       MAKE file,   AeroSim
#//
#//==============================================================================
#6-GRAPHICS/airveh_axis.o \
#			6-GRAPHICS/0-PFD/PFD_Exec.o \
#			6-GRAPHICS/0-PFD/PFD_Heading.o \
#        	6-GRAPHICS/0-PFD/PFD_Circle.o \
#        	6-GRAPHICS/0-PFD/PFD_Bezzel.o \
#        	6-GRAPHICS/0-PFD/PFD_AltTicker.o \
#        	6-GRAPHICS/0-PFD/PFD_Attitude.o \
#        	6-GRAPHICS/0-PFD/PFD_RateOfClimb.o \
#        	6-GRAPHICS/0-PFD/PFD_AltTape.o \
#        	6-GRAPHICS/0-PFD/PFD_SpeedTape.o \
#        	6-GRAPHICS/0-PFD/PFD_SpeedTicker.o \
#        	6-GRAPHICS/0-PFD/PFD_Display.o \
#        	6-GRAPHICS/0-PFD/Flight_Stick.o \
#        	6-GRAPHICS/HSI_Map.o \

PROG   = a
CC     = gcc
CFLAGS = -Wall -g
INCS   = -Iglad/include
LIBS   = -lglfw -lGL -ldl -lpthread -lm -lglut -lGLU

OBJS	= 	6-GRAPHICS/airveh_axis.o \
			6-GRAPHICS/0-PFD/PFD_Exec.o \
			6-GRAPHICS/0-PFD/PFD_Heading.o \
        	6-GRAPHICS/0-PFD/PFD_Circle.o \
        	6-GRAPHICS/0-PFD/PFD_Bezzel.o \
        	6-GRAPHICS/0-PFD/PFD_AltTicker.o \
        	6-GRAPHICS/0-PFD/PFD_Attitude.o \
        	6-GRAPHICS/0-PFD/PFD_RateOfClimb.o \
        	6-GRAPHICS/0-PFD/PFD_AltTape.o \
        	6-GRAPHICS/0-PFD/PFD_SpeedTape.o \
        	6-GRAPHICS/0-PFD/PFD_SpeedTicker.o \
        	6-GRAPHICS/0-PFD/Flight_Stick.o \
        	6-GRAPHICS/HSI_Map.o \
        	6-GRAPHICS/main_graphics.o \
        	4-IPC/Client_TxRx.o \
            5-TM/TM_Print.o \
        	7-MATH/Vector.o \
            7-MATH/Matrix.o \
            7-MATH/Quaternion.o \
			OpenGL-GLAM.o \
			glad/src/glad.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f *~ *.o $(PROG)
	rm -f glad/src/*.o
	rm -f 4-IPC/*.o
	rm -f 5-TM/*.o	
	rm -f 6-GRAPHICS/*.o
	rm -f 7-MATH/*.o	


#=============================================================================
# EOF
	
	

