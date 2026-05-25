#ifndef CVSHADER_H
#define CVSHADER_H

#include <glad/glad.h>
#include <stdbool.h>
#include <cglm/cglm.h>

struct CVShaderProgram {
    GLint programID;
    GLint vertexID;
    GLint fragmentID;
    GLint geometryID;
};

void cvSetFloatMatrix(struct CVShaderProgram *program, const char *name, mat4 matrix);

void cvSetFloatVector(struct CVShaderProgram *program, const char *name, vec4 vector);

void cvSetFloatVector3(struct CVShaderProgram *program, const char *name, vec3 vector);


void cvSetInteger(struct CVShaderProgram *program, const char *name, int integer);

#endif