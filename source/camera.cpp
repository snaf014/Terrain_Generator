#include "camera.hpp"

fps_camera::fps_camera ()
{
   ref = vec3 (0.0f, 0.0f, 1.0f);
   eye = vec3 (0.0f, 1.0f, 0.0f);
   moveSpeed = 2;
   rotateSpeed = 3;
   pitch = 0;
   yaw = 0;
}

fps_camera::fps_camera (vec3 _eye, vec3 _ref)
{
   ref = _ref;
   eye = _eye;
   moveSpeed = 2;
   rotateSpeed = 3;
   pitch = 0;
   yaw = 0;
}

fps_camera::fps_camera (vec3 _eye)
{
   ref = vec3 (0.0f, 0.0f, 1.0f);
   eye = _eye;
   moveSpeed = 2;
   rotateSpeed = 3;
   pitch = 0;
   yaw = 0;
}


void fps_camera::setEyePos (vec3 a)
{
   eye = a;
}

void fps_camera::setRefPos (vec3 a)
{
   ref = a;
}

void fps_camera::setMoveSpeed (float a)
{
   moveSpeed = a;
}

void fps_camera::setRotateSpeed (float a)
{
   rotateSpeed = a;
}

void fps_camera::moveForward (float direction)
{
   vec3 t;
   t = normalize (ref);
   
   eye += t*direction*moveSpeed;
}

void fps_camera::rotatePitch (float direction)
{
   pitch += 3*direction*rotateSpeed;
   ref.y = tand (pitch);
}

void fps_camera::rotateYaw (float direction)
{
   yaw += 3*direction*rotateSpeed;
   ref.x = sind(yaw);
   ref.z = -cosd(yaw);
}

void fps_camera::update ()
{
   gluLookAt (eye.x,eye.y,eye.z, eye.x+ref.x,eye.y+ref.y,eye.z+ref.z, 0.0f,1.0f,0.0f);
}
