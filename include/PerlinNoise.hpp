// GENERATOR SZUMU PERLINA
// ORAZ FUNKCJE PRZYGOTOWUJ�CE DANE TERENU DO WY�WIETLENIA

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
	float value_min, value_max; // minimalna i maksymalna warto�� w tablicy wysoko�ci (do skalowania)
	int size; // rozmiar mapy
	
	float *fData; // zmiennoprzecinkowe dane wysoko�ci
	
	// zmienne do algorytmu szumu perlina
	ubyte perm [512]; // tablica permutacji
	vec2 grad [8]; // tablica gradient�w
	
	public:
	
	usint *data; // mapa wysoko�ci
    sbyte *normalmap; // mapa normalnych
	
	private:
	
	float fade (float); // funkcja interpoluj�ca
	float mix (float, float, float); // interpolacja
	
	public:
	
	PerlinNoise (int);
	void seed (int); // r�czne ustawienia ziarna generatora
	void newSeed (); // automatyczne wygenerowanie nowego ziarna
	void GenMapMultioctave (int, float); // funkcja gyeneruj�ca map� wysoko�ci (suma kilku oktaw szumu perlina)
	void RescaleMap (); // skalowanie wygenerowanych warto�ci do przedzia�u [0, 65535]
	void GenNormals (); // funkcja generuj�ca normalne dla mapy wysoko�ci
	usint *genTexture (int, int, float); // funkcja generuj�ca zestaw danych do wy�wietlanie terenu (mapa wysoko�ci + normalne)
	float gen (float, float); // generowanie pojedynczego punktu wysoko�ci (algorytm Perlina - na podstawie oryginalnej implementacji autora)
};

#endif
