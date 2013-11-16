#include "PerlinNoise.hpp"

float PerlinNoise::fade (float a)
{
	return a*a*a*(a*(a*6-15)+10);
}

float PerlinNoise::mix (float a, float b, float t)
{
	return (1-t)*a+t*b;
}

PerlinNoise::PerlinNoise (int _seed)
{
	seed(_seed);

	grad[0]=vec2(-1.0f,-1.0f);
	grad[1]=vec2(-1.0f,0.0f);
	grad[2]=vec2(-1.0f,1.0f);
	grad[3]=vec2(0.0f,-1.0f);
	grad[4]=vec2(0.0f,1.0f);
	grad[5]=vec2(1.0f,-1.0f);
	grad[6]=vec2(1.0f,0.0f);
	grad[7]=vec2(1.0f,1.0f);
}

void PerlinNoise::seed (int _seed)
{
	srand (_seed);

	for (int i = 0; i < 512; i++)
	{
		perm[i] = rand() % 255;
	}
}

void PerlinNoise::newSeed ()
{
	seed (time (NULL));
}

void PerlinNoise::GenMapMultioctave (int octaves, float presisetence)
{
	float freq = 0.00256, amp = 1;
	float val;
	
	value_min = 99999999;
    value_max = 0;

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			freq = 0.00256;
			amp = 1;
			val = 0;

			for (int i = 0; i < octaves; i++) // sumowanie wartosci szumu z wszystkich oktaw
			{
				val += gen(x*freq,y*freq)*amp;

				freq *= 2;
				amp /=2.2;
			}

			val /= octaves;

			fData [x+y*size] = val;

			if (val > value_max) value_max = val;
			if (val < value_min) value_min = val;
		}
	}
}

void PerlinNoise::RescaleMap ()
{
	float tmp;
	
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			tmp = fData [x+y*size] = (fData [x+y*size] - value_min)/(value_max-value_min);      	
			tmp *= 65535.0f;

			data [x+y*size] = (unsigned short)tmp;
		}
	}
}

void PerlinNoise::GenNormals ()
{
	float nx, ny, nz, ns; // zmienne potrzebne do generowania normalnych
	int p = 0;

	float ratio = size/65536.0f;

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{                    
			// uproszczony algorytm generowania normalnej
			nx = (fData[us(x-1)+y*size] - fData[x+1+y*size]);
			nz = 2*ratio;
			ny = (fData[x+(y+1)*size] - fData[x+us(y-1)*size]);

			//normalizacja normalnej
			ns = sqrt(nx*nx + ny*ny + nz*nz);
			nx /= ns;
			ny /= ns;
			nz /= ns;        

			// konwersja znormalizowanych wartosci z float do byte oraz przypisanie do mapy normalnych
			normalmap[p++] = sbyte(nx*nx*127.0f);
			normalmap[p++] = sbyte(ny*ny*127.0f);
			normalmap[p++] = sbyte(nz*nz*127.0f);
		}
	}
}

usint *PerlinNoise::genTexture (int _size, int _octaves, float _presisetence)
{        
	size = _size;

	fData = new float [size*size];
	data = new usint [size*size]; // mapa wysokosci
	normalmap = new sbyte [3*size*size]; // mapa normalnych

	GenMapMultioctave (_octaves, _presisetence);
	RescaleMap ();
	GenNormals ();

	return data;
}
	  
float PerlinNoise::gen (float x, float y)
{
	int X = (int)x;
	int Y = (int)y;

	x -= X;
	y -= Y;

	X &= 255;
	Y &= 255;

	int gi00 = perm[Y+perm[X]]%8;
	int gi01 = perm[Y+perm[X+1]]%8;
	int gi10 = perm[Y+1+perm[X]]%8;
	int gi11 = perm[Y+1+perm[X+1]]%8;

	float n00 = y*grad[gi00].x + x*grad[gi00].y;
	float n01 = y*grad[gi01].x + (x-1)*grad[gi01].y;
	float n10 = (y-1)*grad[gi10].x + x*grad[gi10].y;
	float n11 = (y-1)*grad[gi11].x + (x-1)*grad[gi11].y;

	float u = fade(x);
	float v = fade(y);

	float nx0 = mix(n00, n01, u);
	float nx1 = mix(n10, n11, u);

	float nxy = mix(nx0, nx1, v);

	return (nxy+1.0f)*32768.0f;
}
