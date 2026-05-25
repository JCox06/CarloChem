#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

struct CVCamera {
    vec3 worldPosition;
    vec3 direction;
    vec3 upVector;

    float fov;

    mat4 view;
    mat4 perspective;
    mat4 project;

};


void cvCameraInit(struct CVCamera *camera, float posX, float posY, float posZ, float dirX, float dirY, float dirZ);

void cvCameraUpdate(struct CVCamera *camera, float aspectRatio);

#endif