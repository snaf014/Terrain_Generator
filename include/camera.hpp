// KAMERA FPS - OPENGL

#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glu.h>
#include <glm/glm.hpp>

using namespace glm;

#include "toollib.hpp"

class fps_camera
{
	public:
	
	float moveSpeed; // prêdkoœæ poruszania kamery do przodu i do ty³u
	float rotateSpeed; // prêdkoœæ obracania kamery

	vec3 ref; // punkt, na który skierowana jest kamera
	vec3 eye; // pozycja kamery

	// k¹ty Eulera obrotu kamery
	float pitch;
	float yaw;

	fps_camera ();
	fps_camera (vec3, vec3);
	fps_camera (vec3);
	void setEyePos (vec3); // ustaw pozycjê kamery
	void setRefPos (vec3); // ustaw punkt, na który skierowana jest kamera
	void setMoveSpeed (float); // ustaw prêdkoœæ
	void setRotateSpeed (float); // ustaw prêdkoœæ obrotu kamery
	void moveForward (float); // przesuñ do przodu/ do ty³u
	void rotatePitch (float); // obróæ kamerê
	void rotateYaw (float); // obróæ kamerê
	void update (); // odœwie¿ pozycjê kamery
};

#endif
