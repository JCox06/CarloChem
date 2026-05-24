#include "CVRender.h"

static void setData(float *vertices, int index, float x, float y, float z) {
    
    vertices[0 + index * VERTEX_COUNT] = x;
    vertices[1 + index * VERTEX_COUNT] = y;
    vertices[2 + index * VERTEX_COUNT] = z;
}

//For now - let a vertex contain just position (x, y, z, w);
void cvCreateRectangleMesh(struct CVMesh *mesh, float x, float y, float dx, float dy) {
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

void cvDeleteMesh(struct CVMesh *mesh) {
    free(mesh->vertices);
    free(mesh->indices);

    mesh->vertexCount = 0;
    mesh->indexCount = 0;
}

void cvCreateVertexArray(struct CVVertexArray *array, const struct CVMesh *mesh) {

    //Create the vertex Array
   GLuint vertexArray, vertexBuffer, indexBuffer;
   glGenVertexArrays(1, &vertexArray);
   glGenBuffers(1, &vertexBuffer);
   glGenBuffers(1, &indexBuffer);

   glBindVertexArray(vertexArray);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, mesh->vertexCount * sizeof(float), mesh->vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexCount * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   //vertex_count is a helpful property that stores the size of the buffer
   //Since OpenGL wants the number of vertices, and each vertex has a position, (and may) have a texture coordinate
   //Division is required
   array->vertices = mesh->indexCount;


   array->vertexArray = vertexArray;
   array->vertexBuffer = vertexBuffer;
   array->indexBuffer = indexBuffer;
}

void cvUseVertexArray(const struct CVVertexArray *array) {
    glBindVertexArray(array->vertexArray);
}

void cvDeleteVertexArray(struct CVVertexArray *array) {
    cvUseVertexArray(array);
    glDeleteVertexArrays(1, &(array->vertexArray));
    int toDelete[] = {array->vertexBuffer, array->indexBuffer};
    glDeleteBuffers(2, toDelete);
    array->vertices = 0;
    array->vertexBuffer = 0;
    array->indexBuffer = 0;
    array->vertexArray = 0;
}