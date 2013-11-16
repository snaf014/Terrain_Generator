#version 120

uniform sampler2D map; // mapa wysokosci terenu
uniform sampler2D normalmap; // mapa normalnych terenu
uniform float height; // maksymalna wysokosc terenu

varying vec3 normal; // normalna powierzchni
varying vec3 lPosition; // pozycja swiatla

// funkcja pobierajaca wysokosc punktu terenu z mapy wysokosci
float getHeight (vec2 pos)
{
	return texture2D (map, pos/512.0).r;
}

// funkcja pobierajaca normalna aktualnego wierzcholka terenu z mapy normalnych
vec3 getNormal (vec2 pos)
{
	return texture2D (normalmap, pos/512.0).rgb;
}

void main ()
{	
	vec4 lightPos = vec4(1000.0, 1000.0, 2560.0, 0.0); // przykladowa pozycja swiatla (tymczasowa)


	// OBLICZANIE WSPOLRZEDNYCH PUNKTU TERENU
	
	vec4 mappoint = gl_Vertex; // wspolrzedne wierzcholka
	float h = getHeight (mappoint.xz);
	mappoint.y = h*height;
	
	gl_Position = gl_ModelViewProjectionMatrix * mappoint;
	
	
	// OBLICZANIE DANYCH DO OSWIETLENIA
	
	normal = getNormal (gl_Vertex.xz)*gl_NormalMatrix;
	
	lPosition = ((lightPos - mappoint)*gl_ModelViewMatrix).xyz; // obliczanie odleglosci swiatla od punktu terenu
	lPosition = normalize (lPosition);
}
 
