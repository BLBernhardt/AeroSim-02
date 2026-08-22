#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>




/*
Circle generates a series of points which lie on the
perimeter of the circle. The circle is evaluated as an arc,
given a start and end point in degrees. Degrees are defined as 0=360 being
up, 90 degrees right, etc.
*/




void Draw_Circle
(
  double XOrigin,  double YOrigin, 
  double radius,   double DegPerPnt, 
  double StartArc, double EndArc      
)

{
  // Add the vertexes specified
  double x;
  double y;

  double pi = 3.14159265;

  // We parametrically evaluate the circle
  // x = sin(t)
  // y = cos(t)
  // t goes from 0 to 2pi
  // 0 degrees = 0rad, 90 degrees = pi/2rad, etc.
  
  double startRad    =  StartArc / 180 * pi; 
  double endRad      =    EndArc / 180 * pi;
  double radPerPoint = DegPerPnt / 180 * pi;

  if (startRad > endRad) endRad += 2*pi;

  double currentRad = startRad;

  do
  {
     x = radius * sin( currentRad ) + XOrigin;
     y = radius * cos( currentRad ) + YOrigin;

     glVertex2d( x, y );

     currentRad += radPerPoint;

  }while(currentRad < endRad);
  
  x = radius*sin(endRad) + XOrigin;
  y = radius*cos(endRad) + YOrigin;

  glVertex2d(x,y);
}
