#include <glad/glad.h>
#include "CVShader.h"
#include <stdio.h>
#include <cglm/cglm.h>



static GLint compileShader(const char *shaderSource, int type) {

    printf("Shader type is %i COMPILING ===== \n%s\n", type, shaderSource);

    const GLchar *sources[] = {shaderSource};

    GLint shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, sources, NULL);
    glCompileShader(shaderID);

    //Check compilation
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        printf("ERROR - Shader Compilation Failed\n");
    } else {
        printf("Shader compilation successful\n");
    }

    return shaderID;
}


static int getUniformLocation(struct CVShaderProgram *program, const char* uniformName) {
    int location = glGetUniformLocation(program->programID, uniformName);

    if (location == -1) {
        printf("ERROR in retrieving uniform location %s\n", uniformName);
    }

    glUseProgram(program->programID);
    return location;
}


void cvLoadShader(struct CVShaderProgram *shaderProgram, const char *vertexSrc, const char* fragSrc, const char* geomSrc) {
    shaderProgram->vertexID = compileShader(vertexSrc, GL_VERTEX_SHADER);

    shaderProgram->fragmentID = compileShader(fragSrc, GL_FRAGMENT_SHADER);
    
    if (geomSrc != NULL) {
        shaderProgram->geometryID = compileShader(geomSrc, GL_GEOMETRY_SHADER);
    } else {
        shaderProgram->geometryID = 0;
    }

    shaderProgram->programID = glCreateProgram();
    glAttachShader(shaderProgram->programID, shaderProgram->vertexID);
    glAttachShader(shaderProgram->programID, shaderProgram->fragmentID);
    glAttachShader(shaderProgram->programID, shaderProgram->geometryID);

    glLinkProgram(shaderProgram->programID);

    int linkSuccess;
    glGetProgramiv(shaderProgram->programID, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        printf("ERROR - Shader linking failed\n");
    } else {
        printf("Shader Program link successful\n");
    }
}


void cvSetFloatMatrix(struct CVShaderProgram *program, const char *name, mat4 matrix) {
    int location = getUniformLocation(program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix[0]);
}


void cvSetInteger(struct CVShaderProgram *program, const char *name, int integer) {
    int location = getUniformLocation(program, name);
    glUniform1i(location, integer);
}



void cvDeleteShader(struct CVShaderProgram *program) {
    glDetachShader(program->programID, program->vertexID);
    glDetachShader(program->programID, program->fragmentID);
    glDetachShader(program->programID, program->geometryID);
    glDeleteShader(program->vertexID);
    glDeleteShader(program->fragmentID);
    glDeleteShader(program->geometryID);
    glDeleteProgram(program->programID);
}
