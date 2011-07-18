#ifndef __STATISTICS__
#define __STATISTICS__

#include <gsl/gsl_rng.h>

#include <simulation.h>

void bootstrap(gsl_rng *gen,simulation_t *sim,int *r_x,int *r_y);
double gibbs_sampler(gsl_rng *gen,simulation_t *sim,int *history_z,int i,int x,int y);


#endif
