#include "CarloChem.h"

#include "CVFramework.h"
#include "CVShader.h"
#include "CVIO.h"
#include "CVRender.h"


static void loadSimpleShader(struct CVShaderProgram *program, const char *vertFile, const char *fragFile) {
    char* vertexSrc = cvReadString(vertFile);
    char* fragSrc = cvReadString(fragFile);
    cvLoadShader(program, vertexSrc, fragSrc, NULL);
    free(vertexSrc);
    free(fragSrc);
}


void startCarloChem() {
    struct CVEngine cvEngine;
    if (!cvInit(&cvEngine, "CarloChem", 800, 600)) {
      printf("FATAL ERROR - CANNOT CONTINUE \n");
      return;
    }   

    struct CVShaderProgram defaultProgram;
    loadSimpleShader(&defaultProgram, "shaders/simpleTexture.vert", "shaders/simpleTexture.frag");

    //Create custom mesh
    struct CVMesh mesh;
    cvCreateRectangleMesh(&mesh, -0.5f, -0.5f, 1.0f, 1.0f);
    struct CVVertexArray vertexArray;
    cvCreateVertexArray(&vertexArray, &mesh);
    cvDeleteMesh(&mesh);

    int metricX, metricY;

    while (cvKeepOpen(&cvEngine)) {
        glClear(GL_COLOR_BUFFER_BIT);
        cvWindowMetrics(&cvEngine, &metricX, &metricY);
        glViewport(0, 0, metricX, metricY);
        

        //Draw the VAO
        cvUseVertexArray(&vertexArray);
        glDrawElements(GL_TRIANGLES, vertexArray.vertices, GL_UNSIGNED_INT, 0);

        cvUpdate(&cvEngine);
    }

    //Cleanup
    cvDeleteVertexArray(&vertexArray);
    cvDeleteShader(&defaultProgram);
    cvShutdown();
}


