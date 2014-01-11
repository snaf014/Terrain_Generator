#include "chunk.hpp"

// wygeneruj dane terenu, gdy fragment ma odpowiedni status
void Chunk::generateDataIfNeeded ()
{
	if (status == CHUNK_WAITING)
	{
		status = CHUNK_GEN_ASSIGNED;
		generator->genTexture (MAP_SIZE, TERRAIN_OCTAVES, 0, pos.x, pos.y);
		status = CHUNK_GEN_READY;
	}
}

// przygotuj dane tekstur OpenGL zawierajace mape wysokosci oraz mape normalnych
void Chunk::generateMapIfNeeded ()
{
	if (status == CHUNK_GEN_READY)
	{
		// mapa wysokosci - dane jako tekstura OpenGL
		glGenTextures(1, &hmapId);
		glBindTexture(GL_TEXTURE_2D, hmapId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, MAP_SIZE, MAP_SIZE, 0, GL_RED, GL_UNSIGNED_SHORT, generator->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
		// mapa normalnych - dane jako tekstura OpenGL
		glGenTextures(1, &normalId);
		glBindTexture(GL_TEXTURE_2D, normalId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, MAP_SIZE, MAP_SIZE, 0, GL_RGB, GL_BYTE, generator->normalmap);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		// mapy gotowe
		status = CHUNK_MAP_READY;
	}
}

// wyswietl fragment terenu o odpowiedniej szczegolowosci (zmienna lod)
void Chunk::render (int lod)
{
	// rzeczywista pozycja chunka (zmienna shadera)
	glUniform3f(glGetUniformLocation(shader->program, "tiletranslation"), (MAP_SIZE)*(pos.x), 0, (MAP_SIZE)*(pos.y));
	
	// identyfikatory map (zmienne shadera)
	glActiveTexture(GL_TEXTURE0);    
	glBindTexture (GL_TEXTURE_2D, hmapId);
	glUniform1i(glGetUniformLocation(shader->program, "map"), 0);
	
	glActiveTexture(GL_TEXTURE0+1);
	glBindTexture (GL_TEXTURE_2D, normalId);
	glUniform1i(glGetUniformLocation(shader->program, "normalmap"), 1);
		    
	// wywolaj odpowiednia liste wyswietlania (tymczasowe rozwiazanie)
	glCallList (list+lod);
}
