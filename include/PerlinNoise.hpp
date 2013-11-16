// GENERATOR SZUMU PERLINA
// ORAZ FUNKCJE PRZYGOTOWUJ¥CE DANE TERENU DO WYŒWIETLENIA

#ifndef PERLINNOISE_HPP
#define PERLINNOISE_HPP

#include <glm/glm.hpp>
#include <time.h>

#include "toollib.hpp"

using namespace glm;

class PerlinNoise
{
	private:
	
	unsigned short value;
	float value_min, value_max; // minimalna i maksymalna wartoœæ w tablicy wysokoœci (do skalowania)
	int size; // rozmiar mapy
	
	float *fData; // zmiennoprzecinkowe dane wysokoœci
	
	// zmienne do algorytmu szumu perlina
	ubyte perm [512]; // tablica permutacji
	vec2 grad [8]; // tablica gradientów
	
	public:
	
	usint *data; // mapa wysokoœci
    sbyte *normalmap; // mapa normalnych
	
	private:
	
	float fade (float); // funkcja interpoluj¹ca
	float mix (float, float, float); // interpolacja
	
	public:
	
	PerlinNoise (int);
	void seed (int); // rêczne ustawienia ziarna generatora
	void newSeed (); // automatyczne wygenerowanie nowego ziarna
	void GenMapMultioctave (int, float); // funkcja gyeneruj¹ca mapê wysokoœci (suma kilku oktaw szumu perlina)
	void RescaleMap (); // skalowanie wygenerowanych wartoœci do przedzia³u [0, 65535]
	void GenNormals (); // funkcja generuj¹ca normalne dla mapy wysokoœci
	usint *genTexture (int, int, float); // funkcja generuj¹ca zestaw danych do wyœwietlanie terenu (mapa wysokoœci + normalne)
	float gen (float, float); // generowanie pojedynczego punktu wysokoœci (algorytm Perlina - na podstawie oryginalnej implementacji autora)
};

#endif
