#ifndef CVRESOURCES_H
#define CVRESOURCES_H

#include "CVShader.h"
#include "CVMesh.h"

#define MAX_PROGRAMS 5
#define MAX_VERTEX_ARRAYS 5

struct CVResources {
    struct CVShaderProgram programs[MAX_PROGRAMS];
    int programSize;

    struct CVVertexArray vertexArrays[MAX_VERTEX_ARRAYS];
    int vertexArraySize;
};


void cvInitResources(struct CVResources *resources);

//Returns the ID of the shader program to be created
int cvLoadShader(struct CVResources *resources, const char *vertexSrc, const char* fragSrc, const char* geomSrc);

void cvDeleteShader(struct CVResources *resources, int programName);

struct CVShaderProgram* cvUseProgram(struct CVResources *resources, int programName);



//Returns the ID of the VAO to be created
int cvCreateVertexArray(struct CVResources *resources, const struct CVMesh *mesh);

void cvDeleteVertexArray(struct CVResources *resources, int arrayName);

struct CVVertexArray* cvUseVertexArray(struct CVResources *resources, int vertexID);


void cvShutdownResources(struct CVResources *resources);
#endif