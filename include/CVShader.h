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


void cvLoadShader(struct CVShaderProgram *program, const char *vertexSrc, const char* fragSrc, const char* geomSrc);

void cvDeleteShader(struct CVShaderProgram *program);

void cvSetFloatMatrix(struct CVShaderProgram *program, const char *name, mat4 matrix);

void cvSetInteger(struct CVShaderProgram *program, const char *name, int integer);

#endif