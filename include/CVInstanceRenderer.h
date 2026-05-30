#ifndef INSTANCE_RENDERER_H
#define INSTANCE_RENDERER_H

#include "CVMesh.h"


//This is the total number of floats that can be sent to the 
//GPU per instance draw call 
#define SHARED_INSTANCE_SIZE 5000

//Every mesh in this OpenGL framework has a fixed vertex layout
//the instancer currently only passes in 3 floats (x, y, z) position offset
//Therefore the number of OpenGl mappings is 3
#define OPENGL_MAPPINGS 3


struct CVInstanceRenderer {
    int sharedVertexBuffer;
};


void cvInstanceRendererInit(struct CVInstanceRenderer *render);

void cvInstanceRendererDestory(struct CVInstanceRenderer *render);

void cvInstanceArray(struct CVInstanceRenderer *render, struct CVVertexArray *array, float const *InstanceData, int dataSize);

void cvBindInstanceToVertexArray(struct CVInstanceRenderer *render);

#endif