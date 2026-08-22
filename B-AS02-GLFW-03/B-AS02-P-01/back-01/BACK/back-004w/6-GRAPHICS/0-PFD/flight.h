





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <GL/glut.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <GL/glut.h>

//#include "gl.h"
//#include "device.h"
#include "math.h"
#include "colors.h"
#include "objects.h"
/*#include <xns/Xzmsg.h>*/




enum{ PAD1, PAD2, PAD3, PAD4, PAD5, PAD6, PAD7, PAD8, PAD9,
       KEYBD, REDRAW, WINQUIT, MOUSE3, MOUSE2, MOUSE1, LEFTARROWKEY,
       RIGHTARROWKEY, UPARROWKEY, DOWNARROWKEY, PAGEUPKEY, PAGEDOWNKEY,
       INSERTKEY, DELKEY, HOMEKEY };

enum{ MOUSEX, MOUSEY };
enum{ DIAL0, DIAL1, DIAL2, DIAL3, DIAL4 };






#define max_int 500000

#define Scoord
#define Object

#define TRUE  1
#define FALSE 0

#define WINGMAN 1

#define START_X 850.0
#define START_Y 0.0
#define START_Z -2050.0
#define START_AZIMUTH 900

#define HEADER_VERSION HEADER(2.4)
#define then
#ifndef NULL
#define NULL 0
#endif
#define IN_BOX(p,llx,urx,llz,urz) \
	(llx <= p -> x && p -> x <= urx && llz <= p -> z && p -> z <= urz)

extern short debug, bitplanes, dogfight, hud, threat_mode;
extern int xasin (), dist_for_lines;
extern float xsin(int), xcos(int), eye_x, eye_y, eye_z;
extern float (*my_ptw)[4];
extern long qread(short *);
extern long qtest(void);

#define MAX_PLANES  16
#define NAME_LENGTH 15





struct zmsg 
{
    int src[3];
};


struct plane 
{
    long planeid;

    char  version;		/* flight version	*/
    char  cmd;			/* type of packet	*/
    short type;			/* plane type		*/
    short alive;
    char  myname[NAME_LENGTH+1];

    unsigned short status;
    unsigned short won;		/* for msgs these 2 shorts */
    unsigned short lost;	/* hold the plane id	*/

    float x;			/* plane position	*/
    float y;
    float z;
    short azimuth;
    short elevation;
    short twist;

    short mstatus;		/* missile data		*/
    float mx;
    float my;
    float mz;
    float last_mx;
    float last_my;
    float last_mz;
    long  mkill;
    float tps;
    int   airspeed;
    int   thrust;
    short wheels;		/* wheel position 	*/
    short elevator;		/* elevator position 	*/
    char  mtype;
    char  place_holder;		/* XXX use later	*/
    short rollers;		/* rollers position 	*/
};

typedef struct plane *Plane;
extern Plane planes[], get_indata(), lookup_plane(), find_closest_plane();
extern Plane *find_plane();

#define DATA_PACKET 0
#define MSG_PACKET 1
#define SUPERKILL_PACKET 2
#define KILL_PACKET 23

#define FOR_EACH_MSG(p,pp) for (pp=messages,p= *pp++; p->alive > 0; p = *pp++)
#define FOR_EACH_PLANE(p,pp) for (pp=planes,p= *pp++; p->alive > 0; p = *pp++)
/*#define PLANE_ID(p) (*(long *) &((p) -> header.src[2]))*/
#define PLANE_ID(p) ((p)->planeid)
#define NULL_PLANE_ID -1

#define TYPE_ROCKET 0
#define TYPE_SIDEWINDER 1
#define TYPE_CANNON 2

/*
 *  ticks per second (frames drawn per second)
 */
#ifdef _4D
//extern int i_tps;
//extern float tps;
#else
#define i_tps tps
//extern int tps;
#endif

#define TPS tps
/*#define TPS 20*/
extern short tick_counter;

#define G_ACC 32.0
#define GRAVITY gravity
/*#define GRAVITY (G_ACC/TPS/TPS)*/

/* hard code it to 20 to be compatable with GL1 versions
#define MEXPLODE (2 * TPS)	*/
#define MEXPLODE 20
#define MFINISH (MEXPLODE + 1)
#define MLIFE (10 * i_tps)
#define MSTART (MFINISH + MLIFE)

extern int view_elevatn;
extern int view_azimuth;
extern int view_rollang;

/* define variables for screen locations */
extern int TERMFLAG;
extern int HSITDISP;
extern int SAMPLING;
extern int NTSCPARM;
extern int POLHEMUS;
extern int LADDER;
extern int STICK;
extern int BLK_WHT;
extern int NVG_GRN;
extern int NTSCPARM;
extern int ABUSCNTL;
extern int DISPFLAG;
extern int VGXFLAG;
extern int DEPTHMAP;
extern int FONTMODE;
extern int xmaxscreen;
extern int ymaxscreen;
extern int xmiddle;
extern int ymiddle;
extern int meter_vllx;
extern int meter_vurx;
extern int meter_vlly;
extern int meter_vury;
extern int fuel_vllx;
extern int meter_lly;
extern int meter_ury;
extern int thrust_llx;
extern int heading_cx;
extern int speed_llx;
extern int climb_llx;
extern int fuel_llx;
extern int hud_margin;

extern float aspect_ratio;
extern int margin;

extern int world_index;
extern short dials;

extern int XMAXSCREEN;
extern int YMAXSCREEN;


#define XMIDDLE (XMAXSCREEN/2)
#define YMIDDLE (YMAXSCREEN/2)

#define XMAXHELO 1023
#define XMIDHELO 512
#define YMAXHELO 767
#define YMIDHELO 384
#define HUD_MARGIN 250

#define NUMBER_WORLDS 2
#define EARTH_SWAMP   0
#define EARTH_RUNWAY  1
#define EARTH_TAXIWAY 2



//========================================================= NEW Stuff 

int xrandom (register int );
 void reset_meters(  );
int make_planes( int );

int pick_plane ();



 void setup_plane1( Plane );
 void setup_plane2( Plane );
 void setup_plane3( Plane );
 void setup_plane4( Plane );
 void setup_plane5( Plane );
 void setup_plane6( Plane );
 void setup_plane7( Plane );
 void setup_plane8( Plane );
 void setup_plane9( Plane );
 void setup_planea( Plane );
 void setup_planeb( Plane );
 void setup_planec( Plane );
 void setup_planed( Plane );
 void setup_planee( Plane );
 void setup_planef( Plane );
 void setup_planeg( Plane );



void mapcolor( int , int , int , int  );






















