#include "GLSLshader.hpp"

GLSLprogram::GLSLprogram ()
{
}
	
GLSLprogram::GLSLprogram (const char *vs, const char *fs)
{
	init (vs, fs);
}

GLSLprogram::~GLSLprogram ()
{
	clean ();
}

void GLSLprogram::clean ()
{
	glDetachShader (program, vs);
	glDetachShader (program, fs);
	glDeleteShader (vs);
	glDeleteShader (fs);
	glDeleteProgram (program);
}

void GLSLprogram::init (const char *vn, const char *fn)
{
	loadFile (vn);
	vs = loadShader (GL_VERTEX_SHADER);
	
	loadFile (fn);
	fs = loadShader (GL_FRAGMENT_SHADER);
	
	program =glCreateProgram ();

	glAttachShader (program, vs);
	glAttachShader (program, fs);
	
	glLinkProgram (program);
	glUseProgram (program);
}

void GLSLprogram::bind ()
{
	glUseProgram (program);
}

void GLSLprogram::unbind ()
{
	glUseProgram (0);
}
	
unsigned int GLSLprogram::loadShader (unsigned int mode)
{
	unsigned int id;
	const char *chsrc [1];
	chsrc [0] = src.c_str();
	
	id = glCreateShader (mode);
	
	glShaderSource (id, 1, chsrc, NULL);
	glCompileShader (id);
	
	char error [1000];
	glGetShaderInfoLog (id, 1000, NULL, error);
	
	if (mode == GL_VERTEX_SHADER)
	{
		printf ("Vertex shader compile status:\n%s\n\n", error);
	}
	else if (mode == GL_FRAGMENT_SHADER)
	{
		printf ("Fragment shader compile status:\n%s\n\n", error);
	}
	else printf ("SHADER ERROR: Unknown type of shader!\n");
	
	
	return id;
}

void GLSLprogram::loadFile (const char *n)
{
	ifstream in (n);
	string line;
	
	if (in.good())
	{
		src = "";
		
		while (!in.eof())
		{
			getline (in, line);
			src += line + '\n';
		}
	}
	else 
	{
		cout << "SHADER ERROR: Cannot load file " << string(n) << "!" << endl;
	}
}
