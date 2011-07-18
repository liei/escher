#ifndef __SIMULATION__
#define __SIMULATION__

typedef struct{ 
  int iterations;  //Number of simulations
  int steps;       //Steps in Monte Carlo

  int x_size;      //Sample size for x
  int x_pos;       //Results for x (positives)
  int y_size;      //Sample size for y
  int y_pos;       //Results for y (positives)
  int samples;   //Sample size for simulation

  double *alpha;
  double *beta;
  
} simulation_t;

simulation_t *init_simulation(simulation_t *sim,int n,int steps,int size_x,int result_x,int size_y,int result_y,int n_samples, ...);
void finalize_simulation(simulation_t *sim);











#endif
