// GENERATOR SZUMU PERLINA
// ORAZ FUNKCJE PRZYGOTOWUJ¥CE DANE TERENU DO WYŒWIETLENIA

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
	float value_min, value_max; // minimalna i maksymalna wartoœæ w tablicy wysokoœci (do skalowania)
	
	float *fData; // zmiennoprzecinkowe dane wysokoœci
	float amp_sum;
	
	public:
	
	usint *data; // mapa wysokoœci
	usint *datas;
    sbyte *normalmap; // mapa normalnych
    float *dataf;
    
	int size; // rozmiar mapy
	
	int offx, offy;
	
	float averange;
	float min;
	float max;
	
	private:
	
	float fade (float); // funkcja interpoluj¹ca
	float mix (float, float, float); // interpolacja
	
	public:
	
	PerlinNoise ();
	~PerlinNoise ();
	void GenMapMultioctave (int, float);  // funkcja gyeneruj¹ca mapê wysokoœci (suma kilku oktaw szumu perlina)
	void RescaleMap (); // skalowanie wygenerowanych wartoœci do przedzia³u [0, 65535]
	void GenNormals (); // funkcja generuj¹ca normalne dla mapy wysokoœci
	void genTexture (int, int, float, int, int); // funkcja generuj¹ca zestaw danych do wyœwietlanie terenu (mapa wysokoœci + normalne)
	float gen (float, float); // generowanie pojedynczego punktu wysokoœci (algorytm Perlina - na podstawie oryginalnej implementacji autora)
	
	void mod ();
};

#endif
