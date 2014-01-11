
GLSLprogram *shader;
GLFWwindow * window;
fps_camera *camera = new fps_camera (vec3(0, 300, 0), vec3(0, 1, 0));
chunkManager *chm = new chunkManager;

// tymczasowe zmienne do obs³ugi mapy wysokosci
GLuint list;

float height = 300.0f;
int RENDER_DISTANCE = 2000; // zmienna okreslajaca jakosc terenu wraz z odlegloscia od kamery (level of details)
int octaves_no = TERRAIN_OCTAVES;
bool terrain_frozen = false; // zamrozenie terenu - nowe fragmenty terenu nie sa generowane ze zmiana pozycji kamery

float camrotspd = 0.5f, cammovspd = 2.0f;
