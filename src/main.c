#include "CVFramework.h"
#include <stdio.h>
#include <stdbool.h>
#include <glad/glad.h>
#include "CVIO.h"
#include "CVShader.h"
#include <stdlib.h>
#include "CVShapes.h"

int main() {
   struct CVEngine cvEngine;


   bool result = cvInit(&cvEngine, "CarloChem", 800, 600);
   if (result == false) {
      printf("FATAL ERROR - CANNOT CONTINUE \n");
      return -1;
   }

   //Load Shaders
   char* vertexSrc = cvReadString("shaders/simpleTexture.vert");
   char* fragSrc = cvReadString("shaders/simpleTexture.frag");

   struct CVShaderProgram program;
   cvLoadShader(&program, vertexSrc, fragSrc, NULL);
   free(vertexSrc);
   free(fragSrc);

   //Create the mesh
   struct CVMesh mesh;
   cvRectangle(&mesh, -0.5f, -0.5f, 1.0f, 1.0f);

   //Create the vertex Array
   GLuint vertexArray, vertexBuffer, indexBuffer;
   glGenVertexArrays(1, &vertexArray);
   glGenBuffers(1, &vertexBuffer);
   glGenBuffers(1, &indexBuffer);

   glBindVertexArray(vertexArray);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, mesh.vertexCount * sizeof(float), mesh.vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indexCount * sizeof(unsigned int), mesh.indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);


   //Use the program
   glUseProgram(program.programID);

   mat4 matrix;
   glm_mat4_identity(matrix);

   cvSetFloatMatrix(&program, "uPerspective", matrix);
   cvSetFloatMatrix(&program, "uModel", matrix);
   cvSetInteger(&program, "uIgnoreTextures", 1);

   int metricX, metricY;

   while (cvKeepOpen(&cvEngine)) {

      glClear(GL_COLOR_BUFFER_BIT);

      cvWindowMetrics(&cvEngine, &metricX, &metricY);
   
      glViewport(0, 0, metricX, metricY);
      
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      cvUpdate(&cvEngine);
   }

   cvDelete(&mesh);
   cvDeleteShader(&program);
   cvShutdown();
}