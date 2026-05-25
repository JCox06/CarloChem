#include "CVResources.h"

void cvInitResources(struct CVResources *resources) {
    resources->programSize = 0;
    resources->vertexArraySize = 0;
}


int addShaderProgram(struct CVResources *resources, struct CVShaderProgram program) {
    if (resources->programSize >= MAX_PROGRAMS - 1) {
        printf("Could not register shader - max shaders exceeded");
        return -1;
    }
    resources->programs[resources->programSize] = program;
    resources->programSize++;
    return resources->programSize - 1;
}

static void removeShaderProgram(struct CVResources *resources, int remove) {
    //Not implemented - Worry when have to come to it but for the moment just delete
    
}

static int addVertexArray(struct CVResources *resources, struct CVVertexArray vertex) {
    if (resources->programSize >= MAX_VERTEX_ARRAYS - 1) {
        printf("Could not register shader - max shaders exceeded");
        return -1;
    }
    resources->vertexArrays[resources->vertexArraySize] = vertex;
    resources->vertexArraySize++;
    return resources->vertexArraySize - 1;
}

static void removeVertexArray(struct CVResources *resources, int remove) {
    //Not implemented - Worry when have to come to it but for the moment just delete
    //At a later date remove it from the list
    
}


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

int cvLoadShader(struct CVResources *resources, const char *vertexSrc, const char* fragSrc, const char* geomSrc) {

    struct CVShaderProgram shaderProgram;

    shaderProgram.vertexID = compileShader(vertexSrc, GL_VERTEX_SHADER);

    shaderProgram.fragmentID = compileShader(fragSrc, GL_FRAGMENT_SHADER);
    
    if (geomSrc != NULL) {
        shaderProgram.geometryID = compileShader(geomSrc, GL_GEOMETRY_SHADER);
    } else {
        shaderProgram.geometryID = 0;
    }

    shaderProgram.programID = glCreateProgram();
    glAttachShader(shaderProgram.programID, shaderProgram.vertexID);
    glAttachShader(shaderProgram.programID, shaderProgram.fragmentID);
    glAttachShader(shaderProgram.programID, shaderProgram.geometryID);

    glLinkProgram(shaderProgram.programID);

    int linkSuccess;
    glGetProgramiv(shaderProgram.programID, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        printf("ERROR - Shader linking failed\n");
    } else {
        printf("Shader Program link successful\n");
    }

    return addShaderProgram(resources, shaderProgram);
}



void cvDeleteShader(struct CVResources *resources, int programName) {
    struct CVShaderProgram *shader = cvUseProgram(resources, programName);
    glDetachShader(shader->programID, shader->vertexID);
    glDetachShader(shader->programID, shader->fragmentID);
    glDeleteShader(shader->vertexID);
    glDeleteShader(shader->fragmentID);

    if (shader->geometryID != 0) {
        glDetachShader(shader->programID, shader->geometryID);
        glDeleteShader(shader->geometryID);
    }
    glDeleteProgram(shader->programID);

    removeShaderProgram(resources, programName);
}


void cvDeleteVertexArray(struct CVResources *resources, int arrayName) {
    struct CVVertexArray *array = cvUseVertexArray(resources, arrayName);
    glDeleteVertexArrays(1, &(array->vertexArray));
    int toDelete[] = {array->vertexBuffer, array->indexBuffer};
    glDeleteBuffers(2, toDelete);
    array->vertices = 0;
    array->vertexBuffer = 0;
    array->indexBuffer = 0;
    array->vertexArray = 0;

    removeVertexArray(resources, arrayName);
}

struct CVShaderProgram* cvUseProgram(struct CVResources *resources, int programName) {
    struct CVShaderProgram *program = &(resources->programs[programName]);
    glUseProgram(program->programID);
    return program;
}

int cvCreateVertexArray(struct CVResources *resources, const struct CVMesh *mesh) {
    struct CVVertexArray array;

       //Create the vertex Array
   GLuint vertexArray, vertexBuffer, indexBuffer;
   glGenVertexArrays(1, &vertexArray);
   glGenBuffers(1, &vertexBuffer);
   glGenBuffers(1, &indexBuffer);

   glBindVertexArray(vertexArray);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, mesh->vertexCount * sizeof(float), mesh->vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexCount * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   //vertex_count is a helpful property that stores the size of the buffer
   //Since OpenGL wants the number of vertices, and each vertex has a position, (and may) have a texture coordinate
   //Division is required
   array.vertices = mesh->indexCount;

   array.vertexArray = vertexArray;
   array.vertexBuffer = vertexBuffer;
   array.indexBuffer = indexBuffer;

   return addVertexArray(resources, array);
}

struct CVVertexArray* cvUseVertexArray(struct CVResources *resources, int vertexID) {
    struct CVVertexArray *array = &(resources->vertexArrays[vertexID]);
    glBindVertexArray(array->vertexArray);
    return array;
}

void cvShutdownResources(struct CVResources *resources) {
    //At the moment this method is only called on shutdown of the application
    //This means it doesn't really matter at the moment if the cvDeleteShader actually does something
    //However in the future it might be a good idea to implement deletion for these OpenGL objects!
    for (int i = 0; i < resources->programSize; i++) {
        cvDeleteShader(resources, i);
        printf("[Resource Manager] Deleting shader %i\n", i);
    }
    for (int i = 0; i < resources->vertexArraySize; i++) {
        cvDeleteVertexArray(resources, i);
        printf("[Resource Manager] Deleting vertex array %i\n", i);
    }
}

