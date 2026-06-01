
#include "CVInstanceRenderer.h"
#include <glad/glad.h>
#include "CVMesh.h"


void cvInstanceRendererInit(struct CVInstanceRenderer *render) {
    glBindVertexArray(0);
    glGenBuffers(1, &(render->sharedVertexBuffer));

    
    glBindBuffer(GL_ARRAY_BUFFER, render->sharedVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, (SHARED_INSTANCE_SIZE * sizeof(float)), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cvInstanceRendererDestory(struct CVInstanceRenderer *render) {
    printf("[Instancer] Shutting Down \n");
    glDeleteBuffers(1, &(render->sharedVertexBuffer));
}


static void drawMesh(int primitive, float const *data, int dataSize, int vertices, int primitiveCount) {
    glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize * sizeof(float), data);
    glDrawElementsInstanced(primitive, vertices, GL_UNSIGNED_INT, 0, primitiveCount);
}

void cvInstanceArray(struct CVInstanceRenderer *render, struct CVVertexArray *array, float  const * instanceData, int dataSize){
    glBindVertexArray(array->vertexArray);
    
    //For now assume that everything will fit into the array
    cvBindInstanceToVertexArray(render);
    drawMesh(array->primitiveMode, instanceData, dataSize, array->vertices, dataSize / OPENGL_MAPPINGS);
}


void cvBindInstanceToVertexArray(struct CVInstanceRenderer *render){
    glBindBuffer(GL_ARRAY_BUFFER, render->sharedVertexBuffer);
}
