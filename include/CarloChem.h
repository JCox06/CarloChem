#ifndef CARLOCHEM_H
#define CARLOCHEM_H


#include "CVCamera.h"
#include <cglm/cglm.h>
#include "CCSimulation.h"

void startCarloChem();




struct CCState {
    struct CVCamera camera;
    struct CCSimulation simulation;
};

#endif