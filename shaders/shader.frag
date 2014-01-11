#version 120

varying vec3 normal;
varying vec3 lPosition;

varying vec4 fogcolor;
varying float fogfactor;

void main ()
{
	// OBLICZANIE OSWIETLENIA

	float fDiffuse = max (0.0, dot(normal, lPosition)); // proste oswietlenie na podstawie normalnej wierzcholka oraz pozycji swiatla (tylko diffuse shading)
	
	vec4 outcolor = mix (fogcolor, vec4(fDiffuse, fDiffuse, fDiffuse, 1.0), fogfactor);
	
	gl_FragColor = outcolor; // wyjsciowy kolor wierzcholka
}
