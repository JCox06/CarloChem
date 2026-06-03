==================CarloChem==================

CarloChem

I have built this program with the intention to learn C. 

CarloChem utilises OpenGL to build (a very simplistic) chemistry Monte Carlo Simulation.

The program models ions against the Coulombic interaction and Lennard-Jones potential and weights random moves with the Boltzmann distribution.

The program does not implement boundary conditions or a simulation cell volume. The particles have unrestricted access throughout space.


==================BUILDING==================

CarloChem uses GLFW, and CGLM - Both of which are fetched using Cmake FetchContent.
CarloChem also uses GLAD (OpenGL function loader) - However this is included directly in the source

CMake is used to fetch GLFW and CGLM, and compile and link CarloChem.

The makefile simplifies this process:
$make build 
$make run