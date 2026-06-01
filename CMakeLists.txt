cmake_minimum_required(VERSION 3.15)
project(MonteCarlo C)

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland support")

##Libraries
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)


# FetchContent_Declare(
#     glm
#     GIT_REPOSITORY https://github.com/g-truc/glm.git
#     GIT_TAG 1.0.3
# )
# FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    cglm
    GIT_REPOSITORY https://github.com/recp/cglm.git
    GIT_TAG v0.9.6
)
FetchContent_MakeAvailable(cglm)

add_executable(CarloChem src/main.c src/CVFramework.c src/CVIO.c src/CVShader.c src/CVMesh.c src/CarloChem.c src/CVResources.c src/CVCamera.c src/CVInstanceRenderer.c src/CCSimulation.c)

add_library(glad lib/glad/src/glad.c)
target_include_directories(glad PUBLIC lib/glad/include)

target_include_directories(CarloChem PRIVATE include)

target_link_libraries(CarloChem PRIVATE
glfw
glad
cglm
)