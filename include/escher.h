#ifndef __ESCHER__
#define __ESCHER__

#include <simulation.h>

extern int parse_sim(simulation_t *sim, char *simfile);

char *  options(int argc, char **argv);
void run_simulation(simulation_t *sim);







#endif
