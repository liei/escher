#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <simulation.h>

simulation_t *init_simulation(simulation_t *sim,int n,int steps,int size_x,int result_x,int size_y,int result_y,int n_samples, ...){
  va_list alist;
  *sim = (simulation_t) {n,steps,size_x,result_x,size_y,result_y,n_samples,
			 (double *) malloc(sizeof(double)*steps),
			 (double *) malloc(sizeof(double)*steps)
  };
  va_start(alist,n_samples);
  for(int i = 0; i < steps; i++)
    sim->alpha[i] = va_arg(alist,double);
  for(int i = 0; i < steps; i++)
    sim->beta[i] = va_arg(alist,double);

  return sim;
}

void finalize_simulation(simulation_t *sim){
  if(sim != NULL){
    if(sim->alpha != NULL)
      free(sim->alpha);
    if(sim->alpha != NULL)
      free(sim->beta);
    free(sim);
  }
}
