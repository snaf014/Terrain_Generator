#version 120

varying vec3 normal;
varying vec3 lPosition;

void main ()
{
	// OBLICZANIE OSWIETLENIA

	float fDiffuse = max (0.0, dot(normal, lPosition)); // proste oswietlenie na podstawie normalnej wierzcholka oraz pozycji swiatla (tylko diffuse shading)
	
	gl_FragColor = vec4 (fDiffuse, fDiffuse, fDiffuse, 1.0); // wyjsciowy kolor wierzcholka
}
