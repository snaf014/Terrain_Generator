#ifndef CHUNKMANAGER_HPP
#define CHUNKMANAGER_HPP

#include <deque>
#include <glm/glm.hpp>
#include "chunk.hpp"
#include "frustum.hpp"

using namespace std;
using namespace glm;

#define MAX_DIM 128 // maksymalny wymiar swiata - tymczasowe rozwiazanie

extern float height;

class chunkManager
{
	public:
	
	ViewingFrustum *f; // klasa pola widocznosci
	
	vec3 cam_pos;
	
	deque <Chunk*> data_gen_queue; // kolejka generatora danych wysokosci
	deque <Chunk*> map_gen_queue; // kolejka generatora map OpenGL
	deque <Chunk*> render_list; // kolejka chunkow do renderowania
	
	deque <Chunk> chunks; // wszystkie chunki
	
	Chunk* world [MAX_DIM][MAX_DIM]; // tablica reprezentujaca caly teren
	
	chunkManager ();
	void refresh (vec3 pos);
	void addChunk (int x, int y);
	void nextGenerateData ();
	void nextGenerateMap ();
	void refreshRenderList ();
	void renderAll ();
	void updateTerrain (int (*lod_func) (int, int), int lod_distance, glm::vec3 &cam_pos);
	
	
};

#endif
