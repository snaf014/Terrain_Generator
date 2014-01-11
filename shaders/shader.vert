#version 120

uniform sampler2D map; // mapa wysokosci terenu
uniform sampler2D normalmap; // mapa normalnych terenu
uniform float height; // maksymalna wysokosc terenu
uniform float MAPSIZE;
uniform vec3 tiletranslation;
uniform vec3 cameraposition;
uniform float density;

varying vec3 normal; // normalna powierzchni
varying vec3 lPosition; // pozycja swiatla

varying vec4 fogcolor;
varying float fogfactor;


// funkcja pobierajaca wysokosc punktu terenu z mapy wysokosci

float getHeight (vec2 pos)
{
	return texture2D (map, pos/MAPSIZE).r;
}

// funkcja pobierajaca normalna aktualnego wierzcholka terenu z mapy normalnych
vec3 getNormal (vec2 pos)
{
	return texture2D (normalmap, pos/MAPSIZE).rgb;
	/*
	float nx = texture2D (map, vec2(max(pos.x-1.0, 0), pos.y)/MAPSIZE).r - texture2D (map, vec2(pos.x+1.0, pos.y)/MAPSIZE).r;
	float ny = texture2D (map, vec2(pos.x, pos.y+1.0)/MAPSIZE).r - texture2D (map, vec2(pos.x, max(pos.y-1.0, 0))/MAPSIZE).r;
	float nz = 1024.0/65536.0;
	
	return normalize(vec3(nx,ny,nz));
	*/
	
}

void main ()
{	
	vec4 lightPos = vec4(1000.0, 1000.0, 2560.0, 0.0); // przykladowa pozycja swiatla (tymczasowa)

	float e = 2.71828182846;
	fogcolor = vec4 (0, 0, 0, 1.0);

	// OBLICZANIE WSPOLRZEDNYCH PUNKTU TERENU
	
	vec4 mappoint = gl_Vertex; // wspolrzedne wierzcholka
	float h = getHeight (mappoint.xz);
	mappoint.y = h*height;
	
	mappoint += vec4(tiletranslation,0.0);
	gl_Position = gl_ModelViewProjectionMatrix * mappoint;
	
	float factor = density * length(gl_Position.xyz);
	fogfactor = pow(e, -factor*factor);
	
	// OBLICZANIE DANYCH DO OSWIETLENIA
	
	normal = getNormal (gl_Vertex.xz)*gl_NormalMatrix;
	
	// lPosition = ((lightPos - mappoint)*gl_ModelViewMatrix).xyz; // obliczanie odleglosci swiatla od punktu terenu
	lPosition = ((lightPos)*gl_ModelViewMatrix).xyz;
	lPosition = normalize (lPosition);
}
 
