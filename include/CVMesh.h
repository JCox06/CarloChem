#ifndef CVSHAPES_H
#define CVSHAPES_H

#include <glad/glad.h>
#include <cglm/cglm.h>
#include <stdlib.h>

#define VERTEX_COUNT 3

struct CVMesh {
    float *vertices;
    int vertexCount;
    int *indices;
    int indexCount; 
};


//For now - let a vertex contain just position (x, y, z, w);
void cvCreateRectangleMesh(struct CVMesh *mesh, float x, float y, float dx, float dy);


void cvDeleteMesh(struct CVMesh *mesh);

struct CVVertexArray {
    GLint vertexArray;
    GLint vertexBuffer;
    GLint indexBuffer;
    int vertices;
};

#endif