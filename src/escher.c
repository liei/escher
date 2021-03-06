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

#ifdef DEBUG
  #define DMSG(msg) printf("%s\n",msg);
#else
  #define DMSG(msg)
#endif

static simulation_t *sim;
static gsl_rng *gen; 
static FILE *outfile;

int main(int argc, char **argv){
  char *simfile = options(argc,argv);
  parse_sim(sim = malloc(sizeof(simulation_t)),simfile);

  gen = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(gen,time(NULL));
  
  run_simulation(sim,outfile);

  DMSG("free_sim");
  finalize_simulation(sim);
  DMSG("free rng");
  gsl_rng_free(gen);
  DMSG("exit program");
  return EXIT_SUCCESS;
}

char * options(int argc, char **argv){
  DMSG("enter options");
  char *simfile;

  int opt = 0;
  while((opt = getopt(argc,argv,"f:o:")) != -1){
    switch(opt){
    case 'f':
      printf("f\n");
      simfile = optarg;
      break;
    case 'o':
      if((outfile = fopen(optarg,"w")) == NULL){
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
  DMSG("leave options\n");
  if(outfile == NULL) 
    outfile = stdout;
  return simfile;
}

void run_simulation(simulation_t *sim,FILE *outfile){
  DMSG("enter run_simulation");
  int *historyZ = malloc(sizeof(int) * (sim->iterations + 1));
  double *historyTheta = malloc(sizeof(double) * (sim->iterations + 1));	

  historyZ[0] = 200;
  

  int x,y,z;
  double theta;


  for(int i = 1; i <= sim->iterations; i++){
    bootstrap(gen,sim,&x,&y);
    historyTheta[i] = gibbs_sampler(gen,sim,historyZ,i,x,y);
  }	
  fprintf(outfile,"Z,\ttheta\n");
  
  for(int i = 1; i <= sim->iterations; i++){
    fprintf(outfile,"[%5d] Successes: %3d\t Theta = %.10f\n",i,historyZ[i],historyTheta[i]);
  }  
  DMSG("leave runsim");
}
