#include "CVShapes.h"

static void setData(float *vertices, int index, float x, float y, float z) {
    
    vertices[0 + index * VERTEX_COUNT] = x;
    vertices[1 + index * VERTEX_COUNT] = y;
    vertices[2 + index * VERTEX_COUNT] = z;
}

//For now - let a vertex contain just position (x, y, z, w);
void cvRectangle(struct CVMesh *mesh, float x, float y, float dx, float dy) {
    mesh->vertexCount = VERTEX_COUNT * 4;
    mesh->vertices = (float*)malloc(mesh->vertexCount * sizeof(float));

    setData(mesh->vertices, 0, x + dx, y + dy, 0.0f); //Top right
    setData(mesh->vertices, 1, x + dx, y, 0.0f); //Bottom Right
    setData(mesh->vertices, 2, x, y, 0.0f); //Bottom Left
    setData(mesh->vertices, 3, x, y + dy, 0.0f); //Top Left


    mesh->indices = (int*)malloc(6 * sizeof(int));
    mesh->indexCount = 6;

    mesh->indices[0] = 0;
    mesh->indices[1] = 1;
    mesh->indices[2] = 3;
    mesh->indices[3] = 1;
    mesh->indices[4] = 2;
    mesh->indices[5] = 3;
}



void cvDelete(struct CVMesh *mesh) {
    free(mesh->vertices);
    free(mesh->indices);

    mesh->vertexCount = 0;
    mesh->indexCount = 0;
}