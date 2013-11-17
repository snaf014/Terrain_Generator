#include "global.hpp"

int octaves_no = 9;

// generacja listy wyswietlania terenu (tymczasowe rozwi¹zanie)
void buildList ()
{
     list = glGenLists (1);
     glNewList (list, GL_COMPILE);
     
     glTranslatef (-MAP_SIZE, 0, -MAP_SIZE);
     
	 for (int j = 0; j < MAP_SIZE-2; j+=2)
        for (int i = 0; i < MAP_SIZE-2; i+=2)
     	{
        	glBegin (GL_TRIANGLE_FAN);
            
            	glVertex3f (i+1,0,j+1);
            	glVertex3f (i,0,j+1);
            	glVertex3f (i,0,j+2);
      	        glVertex3f (i+1,0,j+2);
                glVertex3f (i+2,0,j+2); 
      	        glVertex3f (i+2,0,j+1);
                glVertex3f (i+2,0,j);
                glVertex3f (i+1,0,j);
            	glVertex3f (i,0,j);
            	glVertex3f (i,0,j+1);    
        	glEnd ();
    	}
    	
    	glEndList ();
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
			
			case GLFW_KEY_A: // zlecenie wygenerowania nowej mapy wysokosci
				pn->newSeed();
				newMap = true;
			break;
			
			case GLFW_KEY_T: // zlecenie wygenerowania nowej mapy wysokosci
				if (octaves_no < 9)
				{
					octaves_no++;
					newMap = true;
				}
			break;
			
			case GLFW_KEY_G: // zlecenie wygenerowania nowej mapy wysokosci
				if (octaves_no > 0)
				{
					octaves_no--;
					newMap = true;
				}
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
	//glFrustum(-ar, ar, -1.0, 1.0, 2.0, 10000.0);
	gluPerspective(60,ar,0.1,1000);
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
		
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    

}

float o = 0;

// odswie¿ scenê
void redrawScene ()
{
	renderSetup ();
	
	glClearColor(0,0,0,1);
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
	camera->update ();
	glColor3f (1.0f, 0.0f, 0.0f);
	glCallList (list);
	
    glfwSwapBuffers (window);
}

bool terminateProgram = false;

// g³ówna funkcja obs³ugi renderingu (tymczasowe rozwi¹zanie)
int renderingSystem ()
{
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
    
    buildList ();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    
    camera->setMoveSpeed(2);
    
    int xxx;
    int xxa = 0;
    float xxd = 0;
    double posx, posy;
    int rx, ry;

	// g³ówna pêtla renderingu
    while (!glfwWindowShouldClose (window))
    {
    	xxx = GetTickCount ();
    	
    	// potrzebne odswie¿enie map wysokosci i normalnych
    	if (isloaded)
		{	
    		glActiveTexture(GL_TEXTURE0);
    		glGenTextures(1, &tex);
    		glBindTexture(GL_TEXTURE_2D, tex);
    		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, MAP_SIZE, MAP_SIZE, 0, GL_RED, GL_UNSIGNED_SHORT, mapka);
    		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    		
    		glUniform1i(glGetUniformLocation(shader->program, "map"), 0);
    		
    		glActiveTexture(GL_TEXTURE0 + 1); 
			glGenTextures(1, &texnm);
    		glBindTexture(GL_TEXTURE_2D, texnm);
    		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, MAP_SIZE, MAP_SIZE, 0, GL_RGB, GL_BYTE, mapkanm);
    		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    		
    		glUniform1i(glGetUniformLocation(shader->program, "normalmap"), 1);
    		
    		isloaded = false;
    	}
    	
    	redrawScene (); // odswie¿ scenê
    
    	static float height = 100.0f;
    	
    	// OBS£UGA KLAWIATURY
    	// sterowanie kamer¹
    	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
             	camera->moveForward (1);
    	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
             	camera->moveForward (-1);
        // sterowanie wysokosci¹ terenu
		else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			height++;
		else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
			height--;
   
   		glUniform1f(glGetUniformLocation(shader->program, "height"), height); // przeka¿ maksymaln¹ wysokosæ terenu do shadera
   
   		// OBS£UGA MYSZY
        glfwGetCursorPos (window, &posx, &posy);
   		
		rx = 800/2-(int)posx;
        ry = 600/2-(int)posy;
        
        camera->rotateYaw (-rx/20.0f);
        camera->rotatePitch (ry/20.0f);    
        
        glfwSetCursorPos (window, 800/2, 600/2);
		
    	// SPRAWDZANIE FPS (do gruntownej przeróbki)
    	
		int xxb = GetTickCount()- xxx;
		float xxc = 1000.0f/xxb;
			
		xxd += xxc;
			
		if (xxa++ == 10)
		{
			printf ("FPS: %.0f\n", xxd/10);
			xxa = 0;
			xxd = 0;
		}
    	
    	glfwPollEvents ();
    }
    
    terminateProgram = true;
    
    glfwDestroyWindow (window);
    glfwTerminate();
    return (EXIT_SUCCESS);
	
}

int main () 
{	
	#pragma omp parallel num_threads(2) // zastosowanie wielow¹tkowosi (do rozbudowy)
	{
		#pragma omp sections nowait 
		{
			#pragma omp section // w¹tek renderingu
			{
				printf ("rendering - thread %d\n", omp_get_thread_num ());
				renderingSystem ();
			}	
		}
		
		while (1) // w¹tek generowania terenu
		{
			if (terminateProgram) break;
			
			if (newMap) // zosta³o wys³ane ¿¹danie przebudowy terenu
			{					
				if (pn->genTexture(MAP_SIZE, octaves_no, 0))
				{
					mapka = pn->data;
					mapkanm = pn->normalmap;
					isloaded = true;
					newMap = false;
				}
			}
		}
	}
			
	return 0;
}
