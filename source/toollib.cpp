#include "toollib.hpp"

float sind (float r)
{
      return sin(M_PI*r/180.f);
}

float cosd (float r)
{
      return cos(M_PI*r/180.f);
}

float tand (float r)
{
      return tan(M_PI*r/180.f);
}

float ctgd (float r)
{
      return 1.0f/tan(M_PI*r/180.f);
}

unsigned int unsign (int a)
{
	return (a > 0 ? a : 0);
}

float dec2rad (float dec)
{
      return M_PI * dec / 180;
}

void drawCube (float size, float posx, float posy, float posz)
{
	glScalef (size, size, size);
	glTranslatef (posx, posy, posz);
	
	glBegin(GL_LINE_STRIP);
		glVertex3f (-1,1,1);
		glVertex3f (-1,-1,1);
		glVertex3f (1,-1,1);
		glVertex3f (1,1,1);
		glVertex3f (-1,1,1);
	glEnd ();
	
	glBegin(GL_LINE_STRIP);
		glVertex3f (-1,1,-1);
		glVertex3f (-1,-1,-1);
		glVertex3f (1,-1,-1);
		glVertex3f (1,1,-1);
		glVertex3f (-1,1,-1);
	glEnd ();
	
	glBegin (GL_LINES);
		glVertex3f (-1,1,-1);
		glVertex3f (-1,1,1);
		glVertex3f (-1,-1,-1);
		glVertex3f (-1,-1,1);
		glVertex3f (1,-1,-1);
		glVertex3f (1,-1,1);
		glVertex3f (1,1,-1);
		glVertex3f (1,1,1);
	glEnd ();
}

void drawBox (float _minx, float _maxx, float _miny, float _maxy, float _minz, float _maxz, float posx, float posy, float posz)
{
	float minx = _minx + posx;
	float maxx = _maxx + posx;
	float miny = _miny + posy;
	float maxy = _maxy + posy;
	float minz = _minz + posz;
	float maxz = _maxz + posz;
	
	glBegin(GL_LINE_STRIP);
		glVertex3f (minx,maxy,maxz);
		glVertex3f (minx,miny,maxz);
		glVertex3f (maxx,miny,maxz);
		glVertex3f (maxx,maxy,maxz);
		glVertex3f (minx,maxy,maxz);
	glEnd ();
	
	glBegin(GL_LINE_STRIP);
		glVertex3f (minx,maxy,minz);
		glVertex3f (minx,miny,minz);
		glVertex3f (maxx,miny,minz);
		glVertex3f (maxx,maxy,minz);
		glVertex3f (minx,maxy,minz);
	glEnd ();
	
	glBegin (GL_LINES);
		glVertex3f (minx,maxy,minz);
		glVertex3f (minx,maxy,maxz);
		glVertex3f (minx,miny,minz);
		glVertex3f (minx,miny,maxz);
		glVertex3f (maxx,miny,minz);
		glVertex3f (maxx,miny,maxz);
		glVertex3f (maxx,maxy,minz);
		glVertex3f (maxx,maxy,maxz);
	glEnd ();
	
	/*
    glBegin(GL_QUADS);
    
	    glNormal3f(-1.0f,0.0f,0.0f);
        glVertex3f(0.0f,0.0f,0.0f);
        glVertex3f(0.0f,0.0f,1.0f);
        glVertex3f(1.0f,0.0f,1.0f);
        glVertex3f(1.0f,0.0f,0.0f);

        glNormal3f(0.0f,0.0f,-1.0f);
        glVertex3f(0.0f,0.0f,0.0f);
        glVertex3f(1.0f,0.0f,0.0f);
        glVertex3f(1.0f,1.0f,0.0f);
        glVertex3f(0.0f,1.0f,0.0f);

        glNormal3f(1.0f,0.0f,0.0f);
        glVertex3f(1.0f,1.0f,0.0f);
        glVertex3f(1.0f,1.0f,1.0f);
        glVertex3f(0.0f,1.0f,1.0f);
        glVertex3f(0.0f,1.0f,0.0f);

        glNormal3f(0.0f,0.0f,1.0f);
        glVertex3f(1.0f,1.0f,1.0f);
        glVertex3f(1.0f,0.0f,1.0f);
        glVertex3f(0.0f,0.0f,1.0f);
        glVertex3f(0.0f,1.0f,1.0f);

        glNormal3f(0.0f,1.0f,0.0f);
        glVertex3f(0.0f,0.0f,0.0f);
        glVertex3f(0.0f,1.0f,0.0f);
        glVertex3f(0.0f,1.0f,1.0f);
        glVertex3f(0.0f,0.0f,1.0f);

        glNormal3f(0.0f,-1.0f,0.0f);
        glVertex3f(1.0f,0.0f,0.0f);
        glVertex3f(1.0f,0.0f,1.0f);
        glVertex3f(1.0f,1.0f,1.0f);
        glVertex3f(1.0f,1.0f,0.0f);
        
    glEnd();*/
}
