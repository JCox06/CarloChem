#ifndef CC_SIMULATION_H
#define CC_SIMULATION_H

#define POSITION_COMPONENTS 3

//SIMULATION PARAMETERS
#define RANDOM_MAGNITUDE 15
#define ROOM_TEMP 298

//IMPORTANT PHYSICAL CONSTANTS
#define SIM_COULOMB_CONST (8.988 * pow(10, 9))
#define SIM_ELEMENTARY_CHARGE (1.602 * pow(10, -19))
#define SIM_AVOGADRO_NUMBER (6.022 * pow(10, 23))
#define SIM_BOLTZMANN_CONSTANT (1.3806 * pow(10, -23))

struct CCSimulation {
    int particleCount;

    //x1, y1, z1, x2, y2, z2, ....
    float *positions;

    //e1, e2, e3, e4, ....
    //The elementary charge of each particle
    float *charges;

    //r1, r2, r3, r4, ....
    //The radius of each particle
    float *radii;


    //Each frame the data is collected and sent to this bulky array where it is uploaded directly to the GPU
    float* bulkyArray;


    double lastPotentialEnergy;

    double thermodynamicTemperature;

    int firstRun;
};


void ccSetSimpleIonic(struct CCSimulation *sim, float aCharge, int aCount, float aSize, float bCharge, int bCount, float bSize);

void ccDestorySimulation(struct CCSimulation *sim);

void ccAlignParticlesToLine(struct CCSimulation *sim);

double ccCalculatePairwisePotential(struct CCSimulation * sim);

void ccUpdateSimulation(struct CCSimulation *sim);

//Takes an energy in joules and converts to kJ/mol
double ccCalculateStandardMolarEnergy(struct CCSimulation *sim);

//For later
//void ccSetSimulationCellSize(float x, float y, float z);


#endif