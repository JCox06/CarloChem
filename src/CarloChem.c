#include "CarloChem.h"

#include "CVFramework.h"
#include "CVShader.h"
#include "CVIO.h"
#include "CVMesh.h"
#include "CVCamera.h"
#include "CVInstanceRenderer.h"
#include <GLFW/glfw3.h>
#include <time.h>


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
   cvCreateSphereMesh(&circleMesh, 0.0f, 0.0f, 0.0f, 1.0f, 300, 150);
   cvCreateVertexArray(resources, &(engine->instancer), &circleMesh, GL_TRIANGLES);
   cvDeleteMesh(&circleMesh);
}


static void populateBulkyArray(struct CCState *state) {
   int positionIndex = 0;
   int chargeIndex = 0;
   int radiusIndex = 0;
   for (int i = 0; i < state->simulation.particleCount * 5;) {
      state->simulation.bulkyArray[i++] = state->simulation.positions[positionIndex++];
      state->simulation.bulkyArray[i++] = state->simulation.positions[positionIndex++];
      state->simulation.bulkyArray[i++] = state->simulation.positions[positionIndex++];

      state->simulation.bulkyArray[i++] = state->simulation.charges[chargeIndex++];
      state->simulation.bulkyArray[i++] = state->simulation.radii[radiusIndex++];
   }
}


static void onRenderLoop(struct CVEngine *engine, struct CCState *state) {
   struct CVResources *resources = &(engine->resources);

   //Apply the shader, VAO, and draw!
   mat4 identity;
   glm_mat4_identity(identity);

   vec3 light;
   glm_vec3_one(light);

   struct CVShaderProgram *program = cvUseProgram(resources, SIMPLE_SHADER);
   cvSetFloatMatrix(program, "uPerspective", state->camera.project);
   cvSetInteger(program, "uIgnoreTextures", 1);
   cvSetFloatVector3(program, "uLight", light);
   struct CVVertexArray *vertexArray = cvUseVertexArray(resources, SPHERE_MESH);


   cvSetFloatMatrix(program, "uModel", identity);
   populateBulkyArray(state);
   

   cvInstanceArray(&(engine->instancer), vertexArray, state->simulation.bulkyArray, state->simulation.particleCount * 5);
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
   if (cvKeyDown(engine, GLFW_KEY_L)) {
      cvLockMouseInWindow(engine, true);
   }
   if (cvKeyDown(engine, GLFW_KEY_K)) {
            cvLockMouseInWindow(engine, false);

   }
}


static void handleMouseCameraMovement(struct CVEngine *engine, struct CCState *state) {
      float deltaX = engine->deltaMouseX;
      float deltaY = engine->deltaMouseY;

      float sense = engine->deltaTime * MOUSE_SENSE;

      state->camera.yaw += deltaX * sense;
      state->camera.pitch += deltaY * sense;
}


static void handleSimUpdates(struct CCState *state) {
   ccUpdateSimulation(&(state->simulation));

   double energy = state->simulation.lastPotentialEnergy;

   // printf("Total Interaction Energy: %e J \n", energy);
   printf("Total Molar Interaction Energy %f kJ/mol \n", ccCalculateStandardMolarEnergy(energy));
}

static void onUpdateLoop(struct CVEngine *engine, struct CCState *state) {
   handleKeyboardCameraMovement(engine, state);
   handleMouseCameraMovement(engine, state);
   
   handleSimUpdates(state);
}

void startCarloChem() {
   struct CVEngine engine;
   cvInit(&engine, "CVEngine", 800, 600);
   glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


   loadAssets(&engine);

   struct CCState state;
   cvCameraInit(&(state.camera), 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, -1.0f);

   //Set simulation type
   ccSetSimpleIonic(&(state.simulation), 1, 50, 1, -1, 50, 1);
   ccAlignParticlesToLine(&(state.simulation));

   int metricX, metricY;

   srand(time(0));

   while (cvKeepOpen(&engine)) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      cvWindowMetrics(&engine, &metricX, &metricY);
      glViewport(0, 0, metricX, metricY);
      cvCameraUpdate(&(state.camera), metricX / (float) metricY);

      onUpdateLoop(&engine, &state);
      onRenderLoop(&engine, &state);

      cvUpdate(&engine);
   }

   ccDestorySimulation(&state.simulation);
   cvShutdown(&engine);
}