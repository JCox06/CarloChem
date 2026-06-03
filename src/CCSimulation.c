#include "CCSimulation.h"
#include <stdlib.h>
#include <stdio.h>
#include <cglm/cglm.h>

static void createStorageArrays(struct CCSimulation *sim, int particles) {
    sim->positions = (float*)calloc(particles * POSITION_COMPONENTS, sizeof(float));
    sim->charges = (float*)malloc(particles * sizeof(float));
    sim->radii = (float*)malloc(particles * sizeof(float));
    sim->particleCount = particles;

    sim->bulkyArray = (float*)malloc(particles * 5 * sizeof(float));
}

//aSize and bSize should refer to the LJ colission parameter
void ccSetSimpleIonic(struct CCSimulation *sim, float aCharge, int aCount, float aSize, float bCharge, int bCount, float bSize) {
    createStorageArrays(sim, aCount + bCount);
    //Leave the positions at zero, the Monte Carlo algorithm will move them anyway
    //Set charges
    for (int i = 0; i < aCount; i++) {
        sim->charges[i] = aCharge;
        sim->radii[i] = aSize /2.0f;
    }
    for (int i = aCount; i < (sim->particleCount); i++) {
        sim->charges[i] = bCharge;
        sim->radii[i] = bSize /2.0f;
    }
    sim->lastPotentialEnergy = 0.0;
    sim->thermodynamicTemperature = ROOM_TEMP;
    sim->firstRun = 1;
}


void ccAlignParticlesToLine(struct CCSimulation *sim) {
    for (int i = 0; i < sim->particleCount * 3;) {
        sim->positions[i++] = 0;
        sim->positions[i++] = i * 0.005f;
        sim->positions[i++] = 0;
    }
}



void ccDestorySimulation(struct CCSimulation *sim) {
    free(sim->positions);
    free(sim->charges);
    free(sim->radii);
    free(sim->bulkyArray);
}


void getParticlePos(struct CCSimulation *sim, int particleIndex, float* pos) {
    int positionIndex = POSITION_COMPONENTS * particleIndex;
    pos[0] = (sim->positions[positionIndex++]);
    pos[1] = (sim->positions[positionIndex++]);
    pos[2] = (sim->positions[positionIndex++]);

}


static double calculateCoulombicPotential(struct CCSimulation *sim, int particleAIndex, int particleBIndex, double particleDistance) {
    


    float aCharge = sim->charges[particleAIndex];
    float bCharge = sim->charges[particleBIndex];

    double potential = (1 / particleDistance) * SIM_COULOMB_CONST * (aCharge * (SIM_ELEMENTARY_CHARGE)) * (bCharge * (SIM_ELEMENTARY_CHARGE));

    return potential;
}

static double calculateLJPotential(struct CCSimulation *sim, int particleAIndex, int particleBIndex, double particleDistance) {

    double aSize = sim->radii[particleAIndex];
    double bSize = sim->radii[particleBIndex];
    // double eqmDist = (aSize + bSize) * pow(10, -10); //From angstroms to metres
    // double colissionParam = eqmDist / (pow(2, 1/6));
    
    double col = (aSize + bSize);
    double colissionParam = col * pow(10, -10); //From angstroms to metres
    
    double wellDepth = 1 * pow(10, -10);

    double LJPotential = 4 * wellDepth * (pow((colissionParam / particleDistance), (12)) - pow((colissionParam / particleDistance), (6)));
    return LJPotential;
}

//By default, all params and worldspace is in Angstroms
double ccCalculatePairwisePotential(struct CCSimulation *sim) {
    double totalInteraction = 0.0f;
    for (int i = 0; i < sim->particleCount; i++) {
        for (int j = 0; j < sim->particleCount; j++) {
        //Only proceed if j is greater than i to avoid double counting the interaction
            if (j < i) {
                vec3 particleAPos;
                vec3 particleBPos;
                getParticlePos(sim, i, particleAPos);
                getParticlePos(sim, j, particleBPos);
                double particleDistance = (double)glm_vec3_distance(particleAPos, particleBPos); //In Angstroms
                particleDistance = particleDistance / (powf(10, 10)); //Now in metres


                double coulombic = calculateCoulombicPotential(sim, i, j, particleDistance);
                double LJPotential = calculateLJPotential(sim, i, j, particleDistance);

                //Total interaction equal to coulombic, + LJ + etc for EACH combination of interactions
                totalInteraction += (coulombic + LJPotential);
            }
        }
    }
    return totalInteraction;
}


double ccCalculateStandardMolarEnergy(struct CCSimulation *sim) {
    double totalEnergy = sim->lastPotentialEnergy;
    double energyPerParticle = totalEnergy / sim->particleCount;
    return (energyPerParticle / 1000) * (SIM_AVOGADRO_NUMBER);
}

static double getRandomNumber(int magnitude) {
    return ((double)rand() / RAND_MAX) * (rand() % magnitude);
}

static void makeRandomMove(struct CCSimulation *sim, int *particleSelected, float *xBefore, float* yBefore, float* zBefore) {
    //Generate number between 0 and (particle size - 1)
    *(particleSelected) = rand() % sim->particleCount;
    vec3 pos;
    getParticlePos(sim, *(particleSelected), pos);
    *xBefore = pos[0];
    *yBefore = pos[1];
    *zBefore = pos[2];

    float newX = *xBefore + getRandomNumber(RANDOM_MAGNITUDE);
    float newY = *yBefore + getRandomNumber(RANDOM_MAGNITUDE);
    float newZ = *zBefore + getRandomNumber(RANDOM_MAGNITUDE);

    int posIndex = *(particleSelected) * 3;
    sim->positions[posIndex++] = newX;
    sim->positions[posIndex++] = newY;
    sim->positions[posIndex++] = newZ;
}


static void moveParticle(struct CCSimulation *sim, int particle, float x, float y, float z) {
    int posIndex = particle * 3;
    sim->positions[posIndex++] = x;
    sim->positions[posIndex++] = y;
    sim->positions[posIndex++] = z;
}

static bool acceptSystemChange(double energyBefore, double energyAfter) {
    double deltaE = energyAfter - energyBefore;
    if (energyAfter < energyBefore) {
        return true;
    }

    //Check against Boltzmann Distribution - Generate random number between 0 and 10
    double random = getRandomNumber(10);
    double BoltzmannFactor = exp(-deltaE / (SIM_BOLTZMANN_CONSTANT * ROOM_TEMP));

    if (BoltzmannFactor > random) {
        return true;
    }
    return false;
}

void ccUpdateSimulation(struct CCSimulation *sim) {

    int particleSelected;
    float xBefore, yBefore, zBefore;
    makeRandomMove(sim, &particleSelected, &xBefore, &yBefore, &zBefore); 
    double currentPotential = ccCalculatePairwisePotential(sim);

    bool acceptProposal = acceptSystemChange(sim->lastPotentialEnergy, currentPotential);

    if (sim->firstRun == 1) {
        acceptProposal = true;
        sim->firstRun = 0;
    }

    if (acceptProposal) {
    sim->lastPotentialEnergy = currentPotential;
    } else {
        moveParticle(sim, particleSelected, xBefore, yBefore, zBefore);
    }
}


