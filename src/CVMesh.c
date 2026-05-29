#include "CVMesh.h"
#include <math.h>

#define PI 3.14

static void setData(float *vertices, int index, float x, float y, float z) {
    
    vertices[0 + index * VERTEX_COUNT] = x;
    vertices[1 + index * VERTEX_COUNT] = y;
    vertices[2 + index * VERTEX_COUNT] = z;
}

//For now - let a vertex contain just position (x, y, z, w); (in XY plane)
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

static void calculateCartesian(float radius, float theta, float phi, float *x, float *y, float *z) {
    //Essentially polar coordinates
    *x = radius * sinf(theta) * cosf(phi);
    *y = radius * sinf(theta) * sinf(phi);
    *z = radius * cosf(theta);
}



void cvCreateSphereMesh(struct CVMesh *mesh, float xPos, float yPos, float zPos, float radius, float quantTheta, float quantPhi) {
    //Use parametric equation of sphere (spherical coordinates)
    //To make it easy, fix r at the centre first, and once finished adjust the positions

    //Start with a fixed value of r, and:
    //  a) Loop through theta (rise between xz plane and y axis) 0 - pi
    //  b) Loop through phi (Azimuthal) 0 - 2pi

    float thetaStep = PI / quantTheta;
    float phiStep = (2 * PI) / quantPhi;

    //Store everything in this array (x, y, z) positions
    //For every quantised coordinate
    mesh->vertexCount = (quantTheta + 1) * (quantPhi + 1) * 3;
    mesh->vertices = (float*)malloc( mesh->vertexCount * sizeof(float));

    for (int i = 0; i <= quantTheta; i++) {

        float thetaAngle = PI / 2 - (i * thetaStep); //Start pointing up, and move down as i progresses

        for (int j = 0; j <= quantPhi; j++) {

            float phiAngle = j * phiStep;

            float x;
            float y; 
            float z;
            calculateCartesian(radius, thetaAngle, phiAngle, &x, &y, &z);

            int indexPosition = ((i * quantPhi) + j) * 3;
            mesh->vertices[indexPosition++] = x;
            mesh->vertices[indexPosition++] = y;
            mesh->vertices[indexPosition++] = z;
        }
    }

    mesh->indexCount = ((quantTheta + 1) * (quantPhi + 1)) * 6;
    mesh->indices = (int*)malloc( mesh->indexCount * sizeof(int));

    //Calculate indices
    for (int i = 0; i <= quantTheta; i++) {
        for (int j = 0; j <= quantPhi; j++) {
            int currentIndex = ((i * quantPhi) + j) * 6;
            int indexBelow = (((i + 1) * quantPhi) + j) * 6;
            int indexRight = ((i * quantPhi) + j + 1) * 6;
            int indexBelowAndRight = (((i + 1) * quantPhi) + j + 1) * 6;

            mesh->indices[currentIndex++] = currentIndex;
            mesh->indices[currentIndex++] = indexBelow;
            mesh->indices[currentIndex++] = indexRight;

            mesh->indices[currentIndex++] = indexRight;
            mesh->indices[currentIndex++] = indexBelow;
            mesh->indices[currentIndex++] = indexBelowAndRight;
        }

    }
}


void cvDeleteMesh(struct CVMesh *mesh) {
    free(mesh->vertices);
    free(mesh->indices);
    mesh->vertexCount = 0;
    mesh->indexCount = 0;
}