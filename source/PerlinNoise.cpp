#include "PerlinNoise.hpp"

// zmienne do algorytmu szumu perlina
ubyte pn_perm [512]; // tablica permutacji
// tablica gradientów
vec2 pn_grad [8] = {vec2(-1.0f,-1.0f), vec2(-1.0f,0.0f), vec2(-1.0f,1.0f), vec2(0.0f,-1.0f), vec2(0.0f,1.0f), vec2(1.0f,-1.0f), vec2(1.0f,0.0f), vec2(1.0f,1.0f)};


void initPerlinNoise (int _seed)
{
	srand (_seed);
	
	for (int i = 0; i < 512; i++)
	{
		pn_perm[i] = rand() % 255;
	}
}

float PerlinNoise::fade (float a)
{
	return a*a*a*(a*(a*6-15)+10);
}

float PerlinNoise::mix (float a, float b, float t)
{
	return (1-t)*a+t*b;
}

PerlinNoise::PerlinNoise ()
{	
}

PerlinNoise::~PerlinNoise ()
{	
	delete [] fData;
	delete [] data;
    delete [] normalmap;
}

void PerlinNoise::GenMapMultioctave (int octaves, float presisetence)
{
	float freq = 0.00256, amp = 1;
	float val;
	
	min = 9999999.0f;
    max = 0.0f;

	#pragma omp parallel for schedule(dynamic) private(val, freq, amp) num_threads (3)
	for (int y = 0; y < size+2; y++)
	{
		for (int x = 0; x < size+2; x++)
		{
			freq = 0.00256;
			amp = START_AMP;
			val = 0;
													
			for (int i = 0; i < octaves; i++) // sumowanie wartosci szumu z wszystkich oktaw
			{
				val += gen((x+offx*(size-1))*freq,(y+offy*(size-1))*freq)*amp;

				freq *= 2;
				amp /= DELTA_AMP;
			}
			
			val /= amp_sum;

			dataf [x+y*(size+2)] = val*65536.0f;
			
			averange += val;
			if (val < min) min = val;
			if (val > max) max = val;
		}
	}
	
	for (int j = 1; j < size+1; j++)
		for (int i = 1; i < size+1; i++)
		{
			data [i-1+(j-1)*size] = (usint)dataf[i+j*(size+2)];
		}
	
	averange /= ((size+2)*(size+2));
}

void PerlinNoise::GenNormals ()
{
	float nx, ny, nz, ns; // zmienne potrzebne do generowania normalnych

	float ratio = size/65536.0f;

	#pragma omp parallel for schedule(dynamic) private(nx,ny,nz,ns) num_threads (3)
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{                    
			// uproszczony algorytm generowania normalnej
			nx = (dataf[x+(y+1)*(size+2)] - dataf[x+2+(y+1)*(size+2)]);
			nz = 65536*2*ratio;
			ny = (dataf[x+1+(y+2)*(size+2)] - dataf[x+1+y*(size+2)]);

			//normalizacja normalnej
			ns = sqrt(nx*nx + ny*ny + nz*nz);
			nx /= ns;
			ny /= ns;
			nz /= ns;        

			// konwersja znormalizowanych wartosci z float do byte oraz przypisanie do mapy normalnych
			normalmap[(x+y*size)*3] = sbyte(nx*127.0f);
			normalmap[(x+y*size)*3+1] = sbyte(ny*127.0f);
			normalmap[(x+y*size)*3+2] = sbyte(nz*127.0f);
		}
	}
	
	delete [] dataf;

}

// tymczasowo niewykorzystane - bedzie sluzyc m. inn. do dodania erozji terenu
void PerlinNoise::mod ()
{
	float aa [8];
	
	for (int y = 1; y < size-1; y++)
		for (int x = 1; x < size-1; x++)
		{
			aa[0] = abs(datas [x+y*size]-datas [x+1+y*size]);
			aa[1] = abs(datas [x+y*size]-datas [x-1+y*size]);
			aa[2] = abs(datas [x+y*size]-datas [x+(y+1)*size]);
			aa[3] = abs(datas [x+y*size]-datas [x+(y-1)*size]);
			
			aa[4] = abs(datas [x+y*size]-datas [x-1+(y-1)*size]);
			aa[5] = abs(datas [x+y*size]-datas [x+1+(y-1)*size]);
			aa[6] = abs(datas [x+y*size]-datas [x-1+(y+1)*size]);
			aa[7] = abs(datas [x+y*size]-datas [x+1+(y+1)*size]);
			
			
			data [x+y*size] = (usint) (*std::max_element(aa,aa+8))*10;
			//datas [x+y*size];
		}
}

void PerlinNoise::genTexture (int _size, int _octaves, float _presisetence, int _offx, int _offy)
{ 
	size = _size;
	offx = _offx;
	offy = _offy;

	amp_sum = 0;
	float amp_tmp = START_AMP;
	for (int i = 0; i < _octaves; i++)
	{
		amp_sum += amp_tmp;
		amp_tmp /= DELTA_AMP;
	}

	//datas = new usint [size*size];

    data = new usint [size*size]; // mapa wysokosci
	normalmap = new sbyte [3*size*size]; // mapa normalnych		
	dataf = new float [(size+2)*(size+2)];
		
	GenMapMultioctave (_octaves, _presisetence);
	//mod ();
	GenNormals();
}
	  
float PerlinNoise::gen (float x, float y)
{
	int X = (int)x;
	int Y = (int)y;

	x -= X;
	y -= Y;

	X &= 255;
	Y &= 255;

	int gi00 = pn_perm[Y+pn_perm[X]]%8;
	int gi01 = pn_perm[Y+pn_perm[X+1]]%8;
	int gi10 = pn_perm[Y+1+pn_perm[X]]%8;
	int gi11 = pn_perm[Y+1+pn_perm[X+1]]%8;

	float n00 = y*pn_grad[gi00].x + x*pn_grad[gi00].y;
	float n01 = y*pn_grad[gi01].x + (x-1)*pn_grad[gi01].y;
	float n10 = (y-1)*pn_grad[gi10].x + x*pn_grad[gi10].y;
	float n11 = (y-1)*pn_grad[gi11].x + (x-1)*pn_grad[gi11].y;

	float u = fade(x);
	float v = fade(y);

	float nx0 = mix(n00, n01, u);
	float nx1 = mix(n10, n11, u);

	float nxy = mix(nx0, nx1, v);

	return (nxy+1.0f)/2;
}
