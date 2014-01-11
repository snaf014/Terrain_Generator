#include "headers.hpp"
#include "global.hpp"
#include "config.hpp"

// generacja list wyswietlania zawieraj¹cych bazowe siatki fragmentow terenu (chunk) dla kolejnych poziomow szczegolowosci
// fragment terenu to kwadratowa siatka o zerowej wspolrzednej y - wysokosci punktow terenu sa modyfikowane w programie wierzcholkow
void buildList (int lod_num) // lod_num - liczba wszystkich poziomow szczegolowosi
{		
	int __step = 1;

     list = glGenLists (lod_num);
     
     for (int k = 0; k < lod_num; k++)
     {     	
		glNewList (list+k, GL_COMPILE);
		
		for (int j = 0; j < MAP_SIZE; j+=2*__step)
        for (int i = 0; i < MAP_SIZE; i+=2*__step)
     	{
        	glBegin (GL_TRIANGLE_FAN);
            
            	glVertex3f (i+__step,0,j+__step);
            	glVertex3f (i,0,j+__step);
            	glVertex3f (i,0,j+__step*2);
      	        glVertex3f (i+__step,0,j+__step*2);
                glVertex3f (i+__step*2,0,j+__step*2); 
      	        glVertex3f (i+__step*2,0,j+__step);
                glVertex3f (i+__step*2,0,j);
                glVertex3f (i+__step,0,j);
            	glVertex3f (i,0,j);
            	glVertex3f (i,0,j+__step);
        	glEnd ();
    	}
    	glEndList ();
    	
    	__step *= 2;
	}		
}

// obs³uga b³êdów API
static void errorCallback (int error, const char* desc)
{
	fputs (desc, stderr);
}

// obs³uga klawiszy (pojedyncze klikniêcie)
static void keyCallback (GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE: // wy³¹cz program
				glfwSetWindowShouldClose (window, GL_TRUE);
			break;
			
			case GLFW_KEY_F: // zamrozenie terenu - nowe fragmenty terenu nie sa generowane ze zmiana pozycji kamery
				terrain_frozen = !terrain_frozen;
			break;
			
			     
		}
	}
}

// ustawienia parametrów renderingu
void renderSetup ()
{
	float ar;
	int width, height;
	
	glfwGetFramebufferSize (window, &width, &height);
	ar = width / (float) height;
		
	glViewport (0, 0, width, height);
	
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60, ar, 0.1, 10000);
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    
    
    glUniform3f(glGetUniformLocation(shader->program, "cameraposition"), camera->eye.x, camera->eye.y, camera->eye.z); // pozycja kamery (zmienna shadera)
}

void mouseEvent () // zdarzenie myszki - sterowanie kamera
{
	static int rx, ry;
	static double posx, posy;
	
    glfwGetCursorPos (window, &posx, &posy);
	
	rx = 800/2-(int)posx;
    ry = 600/2-(int)posy;
    
    camera->rotateYaw (-rx/20.0f);
    camera->rotatePitch (ry/20.0f);    
    
    glfwSetCursorPos (window, 800/2, 600/2);
}

float fog_density = 0.001; // poczatkowa gestosc mgly

void keyboardEvent () // zdarzenia klawiatury - wywolywane ciagle, gdy klawisz jest wcisniety
{
	// sterowanie kamer¹
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
         	camera->moveForward (1);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
         	camera->moveForward (-1);
         	
    // sterowanie maksymalna wysokosci¹ terenu
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		height++;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		height--;
		
	// zmiana jakosci terenu (level of details)
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		RENDER_DISTANCE += 100;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		RENDER_DISTANCE -= 100;
		//
	// zmiana intensywnosci mgly
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		fog_density += 0.00001;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		fog_density -= 0.00001;
		
	// zmiana szybkosci poruszania kamery
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		camera->setMoveSpeed(cammovspd += 0.1f);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)//
		camera->setMoveSpeed(cammovspd -= 0.1f);
	
	// zmiana szybkosci obracania kamery
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		camera->setRotateSpeed(camrotspd += 0.1f);
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		camera->setRotateSpeed(camrotspd -= 0.1f);

	glUniform1f(glGetUniformLocation(shader->program, "height"), height); // maksymalna wysokosc terenu (zmienna shadera)
	glUniform1f(glGetUniformLocation(shader->program, "MAPSIZE"), MAP_SIZE); // rozmiar fragmentu terenu (stala shadera)
	glUniform1f(glGetUniformLocation(shader->program, "density"), fog_density); // gestosc mgly (zmienna shadera)
}

int lod_func (int distance, int max_distance) // funkcja wybierajaca poziom szczegolowisci
// na podstawie odleglosci fragmentu terenu od kamery (wartosci dobrane doswiadczalnie - tymczasowe rozwiazanie)
{
	if (distance <= 0.07*max_distance)
		return 0;
	else if (distance <= 0.15*max_distance)
		return 1;
	else if (distance <= 0.3*max_distance)
		return 2;
	else if (distance <= 0.45*max_distance)
		return 3;
	else if (distance <= 0.6*max_distance)
		return 4;
	else if (distance <= 0.75*max_distance)
		return 4;
	else if (distance <= 0.85*max_distance)
		return 5;
	else return 6;
		
}

// odswiezenie sceny
void redrawScene ()
{//
	renderSetup ();

	// czyszczenie buforow	
	glClearColor(0,0,0,1);
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// uzycie shadera terenu
	shader->bind ();

	// aktualizacja pozycji kamery
	camera->update ();
	
	// wyrenderowanie terenu
	chm->updateTerrain (lod_func, RENDER_DISTANCE, camera->eye);
		
    glfwSwapBuffers (window);
}

bool terminateProgram = false;

// g³ówna funkcja obs³ugi renderingu
int renderingSystem ()
{
	// inicjalizacja API
	
	glfwSetErrorCallback (errorCallback);
	
	if (!glfwInit())
        return(EXIT_FAILURE);
        
    window = glfwCreateWindow (800, 600, "Fractal terrain", NULL, NULL);
    if (!window)
    {
    	glfwTerminate ();
    	return (EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent (window);
    
    glfwSetKeyCallback (window, keyCallback);    
    
    GLenum err = glewInit(); 
    if (GLEW_OK != err)
    	printf ("GLEW ERROR: %s\n", glewGetErrorString(err));

	// wczytanie shaderów
	shader = new GLSLprogram;
    shader->init ("../shaders/shader.vert", "../shaders/shader.frag");
    
    buildList (7); // generacja 7 poziomow szczegolowosci bazowego elementu terenu

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// ustawienie szybkosci poruszania kamery
    camera->setMoveSpeed(2);
    
	// g³ówna pêtla renderingu
    while (!glfwWindowShouldClose (window))
    {    	
    	mouseEvent ();
    	keyboardEvent ();
 
 		if (!terrain_frozen) chm->refresh (camera->eye); // odswiezenie stanu managera terenu (ustawienie zadan generacji nowych fragmentow terenu)
    	
    	chm->nextGenerateMap (); // generacja kolejnej mapy wysokosci na podstawie danych z generatora
    	
    	redrawScene (); // odswie¿ scenê
		   		
    	glfwPollEvents (); // obsluga standardowych zdarzen
    }
    
    terminateProgram = true;
    
    glfwDestroyWindow (window);
    glfwTerminate();
    return (EXIT_SUCCESS);
}

int main () 
{
	omp_set_nested(true); // zagniezdzanie watkow omp
	
	initPerlinNoise (time(NULL)); // inicjacja generatora terenu (parametr - seed)

	#pragma omp parallel sections num_threads (2) // podzial na 2 watki: watek wyswietlania i przygotowywania map
	// oraz watek generatora terenu (ktory takze wykorzystuje wielowatkowosc)
	{
		#pragma omp section
		{		
			printf ("rendering - thread %d\n", omp_get_thread_num ());
			renderingSystem ();
		}
		
		#pragma omp section
		{
			while (!terminateProgram)
			{
				chm->nextGenerateData (); // wykonanie nastepnego zadania generatora
			}
		}
	}
			
	return 0;
}
