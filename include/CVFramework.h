#ifndef CVFRAME_H
#define CVFRAME_H


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "CVResources.h"

struct CVEngine {
    GLFWwindow *mainWindow;
    struct CVResources resources;
    float lastFrameTime;
    float deltaTime;

    float lastMouseX;
    float lastMouseY;
    float deltaMouseX;
    float deltaMouseY;
};


bool cvInit(struct CVEngine *engine, const char *name, int width, int height);
void cvShutdown(struct CVEngine *engine);
bool cvKeepOpen(struct CVEngine *engine);
void cvUpdate(struct CVEngine *engine);
void cvWindowMetrics(struct CVEngine *engine, int *metricX, int *metricY);
float cvRunningTime();
bool cvKeyDown(struct CVEngine *engine, int key);
void cvLockMouseInWindow(struct CVEngine *engine, bool flag);
#endif