#include <stdlib.h>
#include <stdio.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <simulation.h>
#include <statistics.h>

void bootstrap(gsl_rng *gen,simulation_t *sim,int *r_x,int *r_y){
  double theta_x,theta_y,u;
  int x = 0, y = 0;

  //Initializing arrays, number of succesess and probability for success		
  theta_x = sim->x_pos / (double)sim->x_size; 
  theta_y = sim->y_pos / (double)sim->y_size; 
   
  for(int i = 0; i < sim->x_size; i++) {
    u = gsl_ran_flat(gen, 0, 1);
    if(u < theta_x)
      x++;
  }
  
  for(int i = 0; i < sim->y_size; i++){
    u = gsl_ran_flat(gen, 0,1);
    if(u < theta_y)
      y++;
  }	
  *r_x = x;
  *r_y = y;
}

double gibbs_sampler(gsl_rng *gen,simulation_t *sim,int *history_z,int i, int x, int y){
  double alpha = sim->steps + sim->y_size+ x - y + history_z[i-1] + 1;  	
  double beta  = sim->samples - history_z[i-1] + sim->x_size - x - sim->y_size+ y - sim->steps + 1;

  for(int i = 0; i < sim->steps; i++){
      alpha += sim->alpha[i];
      beta  += sim->beta[i];
  }

  double theta = gsl_ran_beta(gen,alpha,beta);
  history_z[i] = gsl_ran_binomial(gen, theta, sim->samples);

  return theta;
}
