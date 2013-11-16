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
