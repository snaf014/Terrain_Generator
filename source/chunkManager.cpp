#include "chunkManager.hpp"

chunkManager::chunkManager ()
{
	// czyszczenie tablicy terenu
	for (int i = 0; i < MAX_DIM; i++)
		for (int j = 0; j < MAX_DIM; j++)
			world [i][j] = NULL;
	
	addChunk (0,0);
	
	f = new ViewingFrustum;
}

// sprawdz ktore chunki powinny byc wygenerowane - po zmianie pozycji
void chunkManager::refresh (vec3 pos)
{
	const int radius = 50; // zasieg generowania od kamery
	
	int __x, __y;
	
	cam_pos = pos;
	
	// nad ktorym chunkiem znajduje sie kamera
	int xx = floor(cam_pos.x/(float)MAP_SIZE);
	int yy = floor(cam_pos.z/(float)MAP_SIZE);
	
	// dodaj chunk, ktorym znajduje sie kamera
	addChunk (xx,yy);
	
	// generowanie chunkow wokol kamery
	for (int _radius = 1; _radius < radius; _radius++)
	{
		for (int _y = 0; _y < _radius*2; _y++)
		{
			__x = xx - _radius;
			__y = yy - _radius + _y;
			
			if (__x*__x + __y*__y <= radius*radius) addChunk (__x, __y);
		}
			
		for (int _x = 0; _x < _radius*2; _x++)
		{
			__x = xx - _radius + _x;
			__y = yy + _radius;
			
			if (__x*__x + __y*__y <= radius*radius) addChunk (__x, __y);
		}
			
		for (int _y = 0; _y < _radius*2; _y++)
		{
			__x = xx + _radius;
			__y = yy + _radius - _y;
			
			if (__x*__x + __y*__y <= radius*radius) addChunk (__x, __y);
		}
			
		for (int _x = 0; _x < _radius*2; _x++)
		{
			__x = xx + _radius - _x;
			__y = yy - _radius;
			
			if (__x*__x + __y*__y <= radius*radius) addChunk (__x, __y);
		}				
	}
	
}

// dodaj chunk - zadanie wygenerowania
void chunkManager::addChunk (int x, int y)
{
	if (x >= 0 && y >= 0)
	if (world[x][y] == NULL)
	{
		chunks.push_back(Chunk(x,y));
		data_gen_queue.push_back(&chunks.back());
		
		world[x][y] = &chunks.back();
	}		
}

// generuj nastepne dane wysokosci
void chunkManager::nextGenerateData ()
{
	if (!data_gen_queue.empty())
	{
		Chunk *r;
		r = data_gen_queue.front();
		r->generateDataIfNeeded();
		
		map_gen_queue.push_back(r);
		
		data_gen_queue.pop_front();
	}
}

// generuj nastepna mape wysokosci
void chunkManager::nextGenerateMap ()
{
	if (!map_gen_queue.empty())
	{
		Chunk *r;
		r = map_gen_queue.front();
		r->generateMapIfNeeded();
		
		map_gen_queue.pop_front();
	}
}

// odswiez liste chunkow do wyswietlenia
void chunkManager::refreshRenderList ()
{
	for (int i = 0; i < chunks.size(); i++)
	{
		if (chunks[i].isReady())
		{
			render_list.push_back (&chunks[i]);
		}
	}
}

// wyswietl wszystkie chunki
void chunkManager::renderAll ()
{
	while (!render_list.empty())
	{
		render_list.back()->render(0);
		render_list.pop_back ();
	}
	
	refreshRenderList ();
}

// wyswietl teren
void chunkManager::updateTerrain (int (*lod_func) (int, int), int lod_distance, glm::vec3 &cam_pos)
{
	int dist;
	int render_val;
	int lod_number;
	
	// zmienne do debugowania
	int ren = 0, rall = 0;
	int poly_cnt = 0;
	
	// wyodrebnienie plaszczyzn przycinania obrazu (view frustum) z macierzy przeksztalcen OpenGL
	f->ExtractFrustum();
	
	for (int i = 0; i < chunks.size(); i++)
	{
		if (chunks[i].isReady())
		{
			// rzeczywiste polozenie srodka chunka w przestrzeni
			float chunkposx = chunks[i].pos.x*MAP_SIZE+MAP_SIZE/2.0f;
			float chunkposy = (chunks[i].generator->min+chunks[i].generator->min)/2*height;
			float chunkposz = chunks[i].pos.y*MAP_SIZE+MAP_SIZE/2.0f;
			
			// sprawdz czy prostokat otaczajacy chunk terenu znajduje sie w polu widzenia
			if (f->CubeInFrustum (chunkposx, chunkposy, chunkposz, MAP_SIZE/2, chunks[i].generator->min*height, chunks[i].generator->max*height))
			{			
				// odleglosc chunka od kamery
				dist = sqrt((chunkposx-cam_pos.x)*(chunkposx-cam_pos.x)+(chunkposy-cam_pos.y)*(chunkposy-cam_pos.y)+(chunkposz-cam_pos.z)*(chunkposz-cam_pos.z));
						
				// obliczanie poziomu szczegolowosci na podstawie odleglosci od kamery
				render_val = dist > lod_distance ?  lod_distance : dist;
				lod_number = lod_func (render_val, lod_distance);
				
				// wyswietlenie chunka z odpowiednim poziomem szczegolowosci
				chunks[i].render(lod_number);
				
				// debug
				poly_cnt += MAP_SIZE/pow(2,lod_number); // liczba widocznych trojkatow
				ren++; // widoczne chunki
			}
			rall++; // debug - wszystkie chunki
		}
	}
	
	// debug
	cout << ren << " of " << rall << "; poly count: " << poly_cnt*poly_cnt/1000 << "K" << endl;
}
