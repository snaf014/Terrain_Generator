#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <GL/glew.h>
#include <GL/glfw3.h>

#include "config.hpp"

#include "camera.hpp"
#include "GLSLshader.hpp"
#include "PerlinNoise.hpp"

#include <glm/glm.hpp>

extern GLSLprogram *shader;
extern GLuint list;

using namespace glm;

enum CHUNK_GENERATION_STATUS {CHUNK_WAITING, CHUNK_GEN_ASSIGNED, CHUNK_GEN_READY, CHUNK_MAP_READY};

// Klasa fragmentu terenu
class Chunk
{
	public:
		
	vec2 pos;
	
	// indeksy tekstur OpenGL reprezentujacych mape wysokosci oraz mape normalnych
	GLuint hmapId;
	GLuint normalId;
	
	CHUNK_GENERATION_STATUS status;
	
	// generator terenu
	PerlinNoise *generator;
	
	Chunk (int _x, int _y)
	{		
		generator = new PerlinNoise;
		status = CHUNK_WAITING;
		pos.x = _x;
		pos.y = _y;
	}
	
	// usun dane generacji
	void clearGenerationData ()
	{
		delete generator;
	}
	
	// czy fragment jest gotowy do wyswietlenia
	bool isReady ()
	{
		if (status == CHUNK_MAP_READY) return true;
		else return false;
	}
	
	// wygeneruj dane terenu, gdy fragment ma odpowiedni status
	void generateDataIfNeeded ();
	
	// przygotuj dane tekstur OpenGL zawierajace mape wysokosci oraz mape normalnych
	void generateMapIfNeeded ();
	
	// wyswietl fragment terenu o odpowiedniej szczegolowosci (zmienna lod)
	void render (int lod);
};

#endif
