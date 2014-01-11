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
	
	float moveSpeed; // pr�dko�� poruszania kamery do przodu i do ty�u
	float rotateSpeed; // pr�dko�� obracania kamery

	vec3 ref; // punkt, na kt�ry skierowana jest kamera
	vec3 eye; // pozycja kamery

	// k�ty Eulera obrotu kamery
	float pitch;
	float yaw;

	fps_camera ();
	fps_camera (vec3, vec3);
	fps_camera (vec3);
	void setEyePos (vec3); // ustaw pozycj� kamery
	void setRefPos (vec3); // ustaw punkt, na kt�ry skierowana jest kamera
	void setMoveSpeed (float); // ustaw pr�dko��
	void setRotateSpeed (float); // ustaw pr�dko�� obrotu kamery
	void moveForward (float); // przesu� do przodu/ do ty�u
	void rotatePitch (float); // obr�� kamer�
	void rotateYaw (float); // obr�� kamer�
	void update (); // od�wie� pozycj� kamery
};

#endif
