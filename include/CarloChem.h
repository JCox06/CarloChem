#ifndef CARLOCHEM_H
#define CARLOCHEM_H


#include "CVCamera.h"
#include <cglm/cglm.h>

void startCarloChem();


struct CCState {
    struct CVCamera camera;
    vec3 positions[1000];
};

#endif