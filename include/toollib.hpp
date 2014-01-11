#ifndef TOOLLIB_H
#define TOOLLIB_H

#include <math.h>
#include <GL\gl.h>

typedef unsigned int uint;
typedef int sint;
typedef unsigned short int usint;
typedef unsigned char uchar;
typedef unsigned char ubyte;
typedef const char* c_string;
typedef char sbyte;
typedef unsigned short usint;

// funkcje trygonometryczne przyjmuj¹ce wartoœæ k¹ta w stopniach
float sind (float);
float cosd (float);
float tand (float);
float ctgd (float);

// konwersja stopni na radiany
float dec2rad (float);

// wartiœci ujemne na zero
unsigned int unsign (int);
#define us(a) (((a)<0) ? 0 : (a))

void drawBox (float, float, float, float, float, float, float, float, float); // funkcja pomocnicza
void drawCube (float, float, float, float);

#endif
