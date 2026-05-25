#include <glad/glad.h>
#include "CVShader.h"
#include <stdio.h>
#include <cglm/cglm.h>


static int getUniformLocation(struct CVShaderProgram *program, const char* uniformName) {
    int location = glGetUniformLocation(program->programID, uniformName);

    if (location == -1) {
        printf("ERROR in retrieving uniform location %s\n", uniformName);
    }

    glUseProgram(program->programID);
    return location;
}



void cvSetFloatMatrix(struct CVShaderProgram *program, const char *name, mat4 matrix) {
    int location = getUniformLocation(program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix[0]);
}


void cvSetInteger(struct CVShaderProgram *program, const char *name, int integer) {
    int location = getUniformLocation(program, name);
    glUniform1i(location, integer);
}

void cvSetFloatVector(struct CVShaderProgram *program, const char *name, vec4 vector) {
    int location = getUniformLocation(program, name);
    glUniform4fv(location, 1, vector);
}


void cvSetFloatVector3(struct CVShaderProgram *program, const char *name, vec3 vector) {
    int location = getUniformLocation(program, name);
    glUniform3fv(location, 1, vector);
}
