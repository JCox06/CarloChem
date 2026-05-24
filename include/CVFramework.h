#ifndef CVFRAME_H
#define CVFRAME_H


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdbool.h>

struct CVEngine {
    GLFWwindow *mainWindow;
};


bool cvInit(struct CVEngine *engine, const char *name, int width, int height);
void cvShutdown();
bool cvKeepOpen(struct CVEngine *engine);
void cvUpdate(struct CVEngine *engine);
void cvWindowMetrics(struct CVEngine *engine, int *metricX, int *metricY);

#endif