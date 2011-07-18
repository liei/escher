/***********************************************************************************\
|*Ihis code is an MCMC of a stocastic genetic cascade                               |
|Made by Christian Page July 2011 at NTNU as a subproject at the iGEM competition   |
\***********************************************************************************/

// Inclued all transition probabilitis, and the observed data under condition of stess/no stress

//The cascade is as follows:
//Stess --> ppGpp --| Lac --> lacI --| mCherry
//Where --> promote production of the left operand, and --| inhibites the production

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>


#include <gsl/gsl_rng.h>     //For random numbers in GSL, see www.gnu.org/software/gsl/ for documentation
#include <gsl/gsl_randist.h> //For random numbers from distributions

#include <escher.h>
#include <statistics.h>
#include <simulation.h>
#include <parser.h>

static simulation_t *sim;
static gsl_rng *gen; 

int main(int argc, char **argv){


  char *simfile = options(argc,argv);
  parse_sim(sim = malloc(sizeof(simulation_t)),simfile);

  gen = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(gen,time(NULL));

  run_simulation(sim);

  finalize_simulation(sim);
  gsl_rng_free(gen);

  return EXIT_SUCCESS;
}

char * options(int argc, char **argv){
  char *simfile;

  int opt = 0;
  while((opt = getopt(argc,argv,"f:o:")) != -1){
    switch(opt){
    case 'f':
      printf("f\n");
      simfile = optarg;
      break;
    case 'o':
      if(freopen(optarg,"w",stdout) == NULL){
	fprintf(stderr,"Couldn't write to output file: %s\n",optarg); 
        exit(EXIT_FAILURE);
      }
      break;
    default:
      break;
    }
  }
  if(optind < argc){
    simfile = argv[optind];
  }
  return simfile;
}

void run_simulation(simulation_t *sim){
  int *historyZ = malloc(sizeof(int) * sim->iterations);
  double *historyTheta = malloc(sizeof(double) * sim->iterations);	

  historyZ[0] = 200;
  historyTheta[0] = 0.6;

  int x,y,z;
  double theta;


  for(int i = 1; i <= sim->iterations; i++){
    bootstrap(gen,sim,&x,&y);
    historyTheta[i] = gibbs_sampler(gen,sim,historyZ,i,x,y);
  }	
  fprintf(stdout,"Z,\ttheta\n");
  
  for(int i = 0; i < sim->iterations; i++){
    fprintf(stdout,"[%5d] Successes: %3d\t Theta = %.10f\n",i,historyZ[i],historyTheta[i]);
  }  
}
