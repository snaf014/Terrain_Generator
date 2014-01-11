#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP

#include <GL/gl.h>
#include <math.h>
#include <iostream>

using namespace std;

// klasa obcinania niewidocznych elementow
// na podst. http://www.crownandcutlass.com/features/technicaldetails/frustum.html
// tymczasowe rozwiazanie

class ViewingFrustum
{
	public:
	
	float frustum[6][4]; // plaszczyzny ostroslupa widoku
	
	ViewingFrustum ()
	{		
	}
	
	bool CubeInFrustum (float x, float y, float z, float size, float miny, float maxy);
	bool SphereInFrustum( float x, float y, float z, float radius );
	void ExtractFrustum();
	
};

#endif
