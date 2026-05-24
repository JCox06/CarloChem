#ifndef CVSHAPES_H
#define CVSHAPES_H

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
void cvRectangle(struct CVMesh *mesh, float x, float y, float dx, float dy);

void cvDelete(struct CVMesh *mesh);

#endif