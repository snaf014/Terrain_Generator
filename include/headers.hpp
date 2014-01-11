#include <GL/glew.h>
#include <GL/glfw3.h>

#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

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
#include "chunkManager.hpp"

#include "camera.hpp"

#define FPS
