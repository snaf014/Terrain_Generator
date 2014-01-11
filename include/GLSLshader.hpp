// KLASA DO WCZYTYWANIA I URUCHAMIANIA SHADERÓW

#ifndef GLSL_SHADER_H
#define GLSL_SHADER_H

#include <GL/glew.h>
#include <GL/glfw3.h>

#include <iostream>
#include <fstream>

using namespace std;

class GLSLprogram
{
	private:
		
	string src;
	
	public:
		
	unsigned int vs, fs, program;
	
	GLSLprogram ();
	GLSLprogram (const char*, const char*);
	~GLSLprogram ();
	void clean ();
	void init (const char*, const char*);
	void bind ();
	void unbind ();
	
	private:
	
	unsigned int loadShader (unsigned int);
	void loadFile (const char*);
};

#endif
