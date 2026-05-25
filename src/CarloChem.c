#include "CarloChem.h"

#include "CVFramework.h"
#include "CVShader.h"
#include "CVIO.h"
#include "CVMesh.h"
#include "CVCamera.h"
#include <GLFW/glfw3.h>


//IDs for the OpenGL objects
#define SIMPLE_SHADER 0
#define SQUARE_MESH 0

#define WALK_SPEED 0.5f

static void loadAssets(struct CVEngine *engine) {

   struct CVResources *resources = &(engine->resources);

   //Shader Loading
   char* vertexSrc = cvReadString("shaders/simpleTexture.vert");
   char* fragSrc = cvReadString("shaders/simpleTexture.frag");
   int id = cvLoadShader(resources, vertexSrc, fragSrc, NULL);
   printf("Loaded simple shader with id of %i\n", id);
   free(vertexSrc);
   free(fragSrc);

   //Test Mesh
   struct CVMesh mesh;
   cvCreateRectangleMesh(&mesh, -0.5f, -0.5f, 1.0f, 1.0f);
   cvCreateVertexArray(resources, &mesh);
   cvDeleteMesh(&mesh);
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
   cvSetFloatMatrix(program, "uModel", identity);
   cvSetInteger(program, "uIgnoreTextures", 1);
   cvSetFloatVector3(program, "uLight", light);
   struct CVVertexArray *vertexArray = cvUseVertexArray(resources, SQUARE_MESH);
   glDrawElements(GL_TRIANGLES, vertexArray->vertices, GL_UNSIGNED_INT, 0);

}


static void onUpdateLoop(struct CVEngine *engine, struct CCState *state) {
   float dt = engine->deltaTime;
   vec3 norm;
   glm_vec3_copy(state->camera.direction, norm);
   glm_vec3_norm(norm);
   glm_vec3_scale(norm, WALK_SPEED, norm);

   if (cvKeyDown(engine, GLFW_KEY_W)) {
      glm_vec3_add(state->camera.worldPosition, norm , state->camera.worldPosition);
   }
   if (cvKeyDown(engine, GLFW_KEY_S)) {
      glm_vec3_negate(norm);
      glm_vec3_add(state->camera.worldPosition, norm , state->camera.worldPosition);
   }
}


void startCarloChem() {
   struct CVEngine engine;
   cvInit(&engine, "CVEngine", 800, 600);
   glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


   loadAssets(&engine);

   struct CCState state;
   cvCameraInit(&(state.camera), 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, -1.0f);


   int metricX, metricY;

   while (cvKeepOpen(&engine)) {
      glClear(GL_COLOR_BUFFER_BIT);
      cvWindowMetrics(&engine, &metricX, &metricY);
      glViewport(0, 0, metricX, metricY);
      cvCameraUpdate(&(state.camera), metricX / metricY);

      onUpdateLoop(&engine, &state);
      onRenderLoop(&engine, &state);

      cvUpdate(&engine);
   }

   cvShutdown(&engine);
}


