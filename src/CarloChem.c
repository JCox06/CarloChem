#include "CarloChem.h"

#include "CVFramework.h"
#include "CVShader.h"
#include "CVIO.h"
#include "CVMesh.h"
#include "CVCamera.h"
#include "CVInstanceRenderer.h"
#include <GLFW/glfw3.h>


//IDs for the OpenGL objects
#define SIMPLE_SHADER 0
#define SQUARE_MESH 0

#define SPHERE_MESH 1

#define WALK_SPEED 5.0f
#define MOUSE_SENSE 2.0f;

static void loadAssets(struct CVEngine *engine) {

   struct CVResources *resources = &(engine->resources);

   //Shader Loading
   char* vertexSrc = cvReadString("shaders/simpleTexture.vert");
   char* fragSrc = cvReadString("shaders/simpleTexture.frag");
   int id = cvLoadShader(resources, vertexSrc, fragSrc, NULL);\
   printf("Loaded simple shader with id of %i\n", id);
   free(vertexSrc);
   free(fragSrc);

   //Test Mesh
   struct CVMesh mesh;
   cvCreateRectangleMesh(&mesh, -0.5f, -0.5f, 1.0f, 1.0f);
   cvCreateVertexArray(resources, NULL, &mesh, GL_TRIANGLES);
   cvDeleteMesh(&mesh);

   //Load the circle mesh
   struct CVMesh circleMesh;
   cvCreateSphereMesh(&circleMesh, 0.0f, 0.0f, 0.0f, 1.0f, 250, 250);
   cvCreateVertexArray(resources, &(engine->instancer), &circleMesh, GL_TRIANGLES);
   cvDeleteMesh(&circleMesh);
}


static void onRenderLoop(struct CVEngine *engine, struct CCState *state) {
   struct CVResources *resources = &(engine->resources);

   //Apply the shader, VAO, and draw!
   mat4 identity;
   glm_mat4_identity(identity);

   vec3 light;
   light[0] = (float) sin(cvRunningTime());
   light[1] = 1.0f;
   light[2] = 1.0f;

   struct CVShaderProgram *program = cvUseProgram(resources, SIMPLE_SHADER);
   cvSetFloatMatrix(program, "uPerspective", state->camera.project);
   cvSetInteger(program, "uIgnoreTextures", 1);
   cvSetFloatVector3(program, "uLight", light);
   struct CVVertexArray *vertexArray = cvUseVertexArray(resources, SPHERE_MESH);


   // for (int i = 0; i < 1000; i++) {
   //    mat4 translation;
   //    glm_mat4_identity(translation);
   //    float *pos = state->positions[i];
   //    glm_translate(translation, pos);
   //    cvSetFloatMatrix(program, "uModel", translation);
   //    glDrawElements(vertexArray->primitiveMode, vertexArray->vertices, GL_UNSIGNED_INT, 0);
   // }



   float positions[3000];
   for (int i = 0; i < 1000; i++) {
      int currentIndex = i;
      float *pos = state->positions[i];
      positions[currentIndex++] = pos[0];
      positions[currentIndex++] = pos[1];
      positions[currentIndex++] = pos[2];
   }
   mat4 translation;
   glm_mat4_identity(translation); 
   cvSetFloatMatrix(program, "uModel", translation);
   cvInstanceArray(&(engine->instancer), vertexArray, positions, 3000);
}



static void handleKeyboardCameraMovement(struct CVEngine *engine, struct CCState *state) {
   float moveScale = engine->deltaTime * WALK_SPEED;
   vec3 movementVector;
   glm_vec3_zero(movementVector);
   if (cvKeyDown(engine, GLFW_KEY_W)) {
      glm_vec3_scale(state->camera.direction, moveScale, movementVector);
      glm_vec3_add(state->camera.worldPosition, movementVector , state->camera.worldPosition);
   }
   if (cvKeyDown(engine, GLFW_KEY_S)) {
      glm_vec3_scale(state->camera.direction, -moveScale, movementVector);
      glm_vec3_add(state->camera.worldPosition, movementVector , state->camera.worldPosition);
   }

   if (cvKeyDown(engine, GLFW_KEY_D)) {
      glm_vec3_scale(state->camera.rightVector, moveScale, movementVector);
      glm_vec3_add(state->camera.worldPosition, movementVector , state->camera.worldPosition);
   }
   if (cvKeyDown(engine, GLFW_KEY_A)) {
      glm_vec3_scale(state->camera.rightVector, -moveScale, movementVector);
      glm_vec3_add(state->camera.worldPosition, movementVector , state->camera.worldPosition);
   }
   if (cvKeyDown(engine, GLFW_KEY_SPACE)) {
      glm_vec3_scale(state->camera.upVector, moveScale, movementVector);
      glm_vec3_add(state->camera.worldPosition, movementVector , state->camera.worldPosition);
   }
   if (cvKeyDown(engine, GLFW_KEY_LEFT_SHIFT)) {
      glm_vec3_scale(state->camera.upVector, -moveScale, movementVector);
      glm_vec3_add(state->camera.worldPosition, movementVector , state->camera.worldPosition);
   }
  
}


static void handleMouseCameraMovement(struct CVEngine *engine, struct CCState *state) {
      float deltaX = engine->deltaMouseX;
      float deltaY = engine->deltaMouseY;

      float sense = engine->deltaTime * MOUSE_SENSE;

      state->camera.yaw += deltaX * sense;
      state->camera.pitch += deltaY * sense;
}

static void onUpdateLoop(struct CVEngine *engine, struct CCState *state) {
   handleKeyboardCameraMovement(engine, state);
   handleMouseCameraMovement(engine, state);
   if (cvKeyDown(engine, GLFW_KEY_L)) {
      cvLockMouseInWindow(engine, true);
   }
   if (cvKeyDown(engine, GLFW_KEY_K)) {
            cvLockMouseInWindow(engine, false);

   }
}




void startCarloChem() {
   struct CVEngine engine;
   cvInit(&engine, "CVEngine", 800, 600);
   glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


   loadAssets(&engine);

   struct CCState state;
   cvCameraInit(&(state.camera), 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, -1.0f);



    //Init the Positions of spheres
   for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
         for (int k = 0; k < 10; k++) {
            int index = i * 100 + j * 10 + k;
            state.positions[index][0] = i * 2.5f;
            state.positions[index][1] = j * 2.5f;        
            state.positions[index][2] = k * 2.5f;        
         }
      }
   }


   int metricX, metricY;

   while (cvKeepOpen(&engine)) {
      glClear(GL_COLOR_BUFFER_BIT);
      cvWindowMetrics(&engine, &metricX, &metricY);
      glViewport(0, 0, metricX, metricY);
      cvCameraUpdate(&(state.camera), metricX / (float) metricY);

      onUpdateLoop(&engine, &state);
      onRenderLoop(&engine, &state);

      cvUpdate(&engine);
   }

   cvShutdown(&engine);
}


