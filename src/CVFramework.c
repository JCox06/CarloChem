#include <glad/glad.h>
#include "CVFramework.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include "CVResources.h"


static bool prepareGLFW() {

    bool status = glfwInit();

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    return status;
}


bool cvInit(struct CVEngine *engine, const char *name, int width, int height) {
    
    bool status = prepareGLFW();

    if (status == false) {
        printf("Could not initialise GLFW\n");
        return false;
    }
    
    engine->mainWindow = glfwCreateWindow(width, height, name, NULL, NULL);

    if (!engine->mainWindow) {
        cvShutdown(engine);
        printf("Could not create main engine GLFW window\n");
        return false;
    }

    glfwMakeContextCurrent(engine->mainWindow);


    int version = gladLoadGL();
    if (version == 0) {
        printf("Failed to initialise OpenGL functions \n");
        return false;
    }

    glClearColor(0.11f, 0.11f, 0.11f, 1.0f);

    printf("Success! GLFW and OpenGL context created\n");

    struct CVResources res;
    cvInitResources(&res);
    engine->resources = res;

    engine->lastFrameTime = 0.0f;
    engine->deltaTime = 0.0f;


    struct CVInstanceRenderer instancer;
    cvInstanceRendererInit(&instancer);
    engine->instancer = instancer;

    return true;
}

void cvShutdown(struct CVEngine *engine) {
    printf("CVEngine is Shutting down \n");
    cvShutdownResources(&(engine->resources));
    cvInstanceRendererDestory(&(engine->instancer));
    glfwTerminate();
}

bool cvKeepOpen(struct CVEngine *engine) {
    return !glfwWindowShouldClose(engine->mainWindow);
}


void cvUpdate(struct CVEngine *engine) {
    glfwSwapBuffers(engine->mainWindow);
    glfwPollEvents();

    //Calculate the delta time
    float currentTime = cvRunningTime();
    engine->deltaTime = currentTime - engine->lastFrameTime;
    engine->lastFrameTime = currentTime;

    //Calculate the delta mouse
    double xCurrentPos, yCurrentPos;
    glfwGetCursorPos(engine->mainWindow, &xCurrentPos, &yCurrentPos);
    engine->deltaMouseX = (float) xCurrentPos - engine->lastMouseX;
    engine->deltaMouseY = -(float) yCurrentPos + engine->lastMouseY; //GLFW coordinates run backwards
    engine->lastMouseX = (float) xCurrentPos;
    engine->lastMouseY = (float) yCurrentPos;
}


void cvWindowMetrics(struct CVEngine *engine, int *metricX, int *metricY) {
    glfwGetFramebufferSize(engine->mainWindow, metricX, metricY);
}

float cvRunningTime() {
    return (float) glfwGetTime();
}


bool cvKeyDown(struct CVEngine *engine, int key) {
    return glfwGetKey(engine->mainWindow, key) == GLFW_PRESS;
}

void cvLockMouseInWindow(struct CVEngine *engine, bool flag) {
    if (flag) {
        glfwSetInputMode(engine->mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (!flag) {
                glfwSetInputMode(engine->mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
