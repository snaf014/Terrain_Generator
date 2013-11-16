#include <GL/glew.h>
#include <GL/glfw3.h>

#include <iostream>
#include <fstream>

using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <windows.h>

#include "toollib.hpp"
#include "GLSLshader.hpp"
#include "PerlinNoise.hpp"
#include "camera.hpp"

GLSLprogram *shader;

GLFWwindow * window;

fps_camera *camera = new fps_camera (vec3(0, 300, 0), vec3(0, 1, 0));
PerlinNoise *pn = new PerlinNoise (time (NULL));

// tymczasowe zmienne do obs³ugi mapy wysokosci
GLuint list;
GLuint tex;
GLuint texnm; 
usint *mapka;
sbyte *mapkanm;

bool isloaded = false;

bool newMap = true; 

#define MAP_SIZE 512 // rozmiar mapy
#define TERRAIN_OCTAVES 9 // ilosc oktaw mapy