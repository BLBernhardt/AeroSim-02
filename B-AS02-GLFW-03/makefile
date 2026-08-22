#//345678901234567890123456789012345678901234567890123456789012345678901234567890
#//==============================================================================
#//
#//	   MAKE file,   AeroSim
#//
#//==============================================================================
#			5-UTIL/Intf_AS_GD.o \

PROG   	= a
CC	 	= gcc
CFLAGS 	= -Wall -g
INCS   	= -Iglad/include
LIBS   	= -lglfw -lGL -ldl -lpthread -lm -lglut -lGLU

OBJS	= 	2-MODEL/aero_model.o \
			2-MODEL/coeff_table.o \
			3-AeroSim/Aero_Simulation.o \
			4-IPC/Client_TxRx.o \
			5-UTIL/TM_Print.o \
		 	5-UTIL/keyboard.o \
			5-UTIL/Mouse.o \
			5-UTIL/Intf_Local_B.o \
			6-GRAPHICS/airveh_axis.o \
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
			6-GRAPHICS/Panel.o \
			main.o \
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
	rm -f 2-MODEL/*.o	
	rm -f 3-AeroSim/*.o
	rm -f 4-IPC/*.o  
	rm -f 5-UTIL/*.o		
	rm -f 6-GRAPHICS/*.o 
	rm -f 6-GRAPHICS/0-PFD/*.o
	rm -f 7-MATH/*.o 
 


#=============================================================================
# EOF
	
	

