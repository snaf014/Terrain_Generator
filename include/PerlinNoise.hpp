// GENERATOR SZUMU PERLINA
// ORAZ FUNKCJE PRZYGOTOWUJ�CE DANE TERENU DO WY�WIETLENIA

#ifndef PERLINNOISE_HPP
#define PERLINNOISE_HPP

#include <algorithm>

#include <glm/glm.hpp>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

#include "toollib.hpp"

using namespace glm;

void initPerlinNoise (int);

#define START_AMP 0.9
#define DELTA_AMP 2.2

class PerlinNoise
{
	private:
	
	unsigned short value;
	float value_min, value_max; // minimalna i maksymalna warto�� w tablicy wysoko�ci (do skalowania)
	
	float *fData; // zmiennoprzecinkowe dane wysoko�ci
	float amp_sum;
	
	public:
	
	usint *data; // mapa wysoko�ci
	usint *datas;
    sbyte *normalmap; // mapa normalnych
    float *dataf;
    
	int size; // rozmiar mapy
	
	int offx, offy;
	
	float averange;
	float min;
	float max;
	
	private:
	
	float fade (float); // funkcja interpoluj�ca
	float mix (float, float, float); // interpolacja
	
	public:
	
	PerlinNoise ();
	~PerlinNoise ();
	void GenMapMultioctave (int, float);  // funkcja gyeneruj�ca map� wysoko�ci (suma kilku oktaw szumu perlina)
	void RescaleMap (); // skalowanie wygenerowanych warto�ci do przedzia�u [0, 65535]
	void GenNormals (); // funkcja generuj�ca normalne dla mapy wysoko�ci
	void genTexture (int, int, float, int, int); // funkcja generuj�ca zestaw danych do wy�wietlanie terenu (mapa wysoko�ci + normalne)
	float gen (float, float); // generowanie pojedynczego punktu wysoko�ci (algorytm Perlina - na podstawie oryginalnej implementacji autora)
	
	void mod ();
};

#endif
