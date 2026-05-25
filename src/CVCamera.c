#include "CVCamera.h"



void cvCameraInit(struct CVCamera *camera, float posX, float posY, float posZ, float dirX, float dirY, float dirZ) {
    camera->worldPosition[0] = posX;
    camera->worldPosition[1] = posY;
    camera->worldPosition[2] = posZ;
    camera->direction[0] = dirX;
    camera->direction[1] = dirY;
    camera->direction[2] = dirZ;

    camera->fov = 45;
    camera->upVector[0] = 0.0f;
    camera->upVector[1] = 1.0f;
    camera->upVector[2] = 0.0f;

    glm_mat4_identity(camera->view);
    glm_mat4_identity(camera->perspective);
    glm_mat4_identity(camera->project);
}

void cvCameraUpdate(struct CVCamera *camera, float aspectRatio) {
    vec3 at;
    glm_vec3_add(camera->direction, camera->worldPosition, at);
    glm_lookat(camera->worldPosition, at, camera->upVector, camera->view);
    glm_perspective(glm_rad(camera->fov), aspectRatio, 0.1f, 100.0f, camera->perspective);
    glm_mat4_mul(camera->perspective, camera->view, camera->project);
}
