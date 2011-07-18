////////////////////////////////////////////////////////////////////////////////////
//Ihis code is an MCMC of a stocastic genetic cascade                             
//Made by Christian Page July 2011 at NTNU as a subproject at the iGEM competition
/////////////////////////////////////////////////////////////////////////////
// Inclued all transition probabilitis, and the observed data under condition of stess/no stress

//The cascade is as follows:
//Stess --> ppGpp --| Lac --> lacI --| mCherry
//Where --> promote production of the left operand, and --| inhibites the production


//Including librarys
#include <stdio.h>
#include <math.h>
#include <cstdlib> //For random numbers
#include <string>

#include <gsl/gsl_rng.h> //For random numbers in GSL, see www.gnu.org/software/gsl/ for documentation
#include <gsl/gsl_randist.h> //For random numbers from distributions


#include <iostream> 
#include <fstream>
#include <iomanip>
using namespace std;

//////////////////////////////////////////////////////////////////////////
//		DECLEARING GLOBALE CONSTANTS				//
//////////////////////////////////////////////////////////////////////////
const int   NUMBER_XYZ[3]				= {200,200,200};
const int   NUMBER_OF_SIMULATIONS			= 15000; 
const int   SUCCESSES_IN_XY[2]				= {190,200};
const int   NUMBER_OF_STEPS_IN_MC			= 3; 
const float TRANSITION_PROB_ALPHA[NUMBER_OF_STEPS_IN_MC]= {20,20,20}; 
const float TRANSITION_PROB_BETA[NUMBER_OF_STEPS_IN_MC] = {0.1,0.1,0.1};
//////////////////////////////////////////////////////////////////////////
//  FUNCTIONS USED IN THIS PROGRAM; SEE SEPERATE FILE FOR DOCUMENTATION	//
//////////////////////////////////////////////////////////////////////////
double* gibbsSampler(int successesZ, int *successesXY);
int*    bootstrap();
void    deletePointer(int *successes, double *returnValue);
static  gsl_rng *gen; 

/////////////	RUNNING PROGRAM	//////////////////////////////////////////				
int main()
{	
	gen = gsl_rng_alloc(gsl_rng_mt19937);
	gsl_rng_set(gen,time(NULL));
	
	ofstream outStream;

	int*    historyZ     = new int [NUMBER_OF_SIMULATIONS];
	double* historyTheta = new double[NUMBER_OF_SIMULATIONS];	

//	int     successesZ = 200;
	int*    samples;
		historyZ[0] = 200;
 		historyTheta[0] = 0.6;
	
	double * temp;	
	
		for(int i = 1; i <= NUMBER_OF_SIMULATIONS; i++)
		{
			samples = bootstrap();
			temp = gibbsSampler(historyZ[i -1], samples);
			
			historyTheta[i] = temp[0];
			historyZ[i]     = temp[1];	 
			deletePointer(samples, temp);
		}	
			
	outStream.open("Output.txt");			
		if(outStream.fail() == true)
		{
			cout << "Outstream fail, file does not exist" << endl;
			outStream.close();
			exit(1);
		}
	
	outStream << "Z" << ", \t" << "theta" << endl; 	
	
		for(int i = 0; i < NUMBER_OF_SIMULATIONS; i++)
		{
			outStream << setprecision(10) << historyZ[i] << ", \t" << historyTheta[i] << endl;
			cout << "Number of Sucess: " << historyZ[i] << "\t Theta = " << historyTheta[i] << endl;
		}



	outStream.close();
	gsl_rng_free(gen);
	
	
	return 0; //End of program

}

//////////////////////////////////////////////////////////////////////////
//		BUILDING FUNCTIONS					//
//////////////////////////////////////////////////////////////////////////
int* bootstrap(void){
  int *successes = new int[2];
  double theta[2]; 
  //INITIALIZING ARRAYS, NUMBER OF SUCCESESS AND PROBABILITY FOR SUCCESS		
  successes[0] = 0;	
  successes[1] = 0;			
  theta[0]= static_cast <double>(SUCCESSES_IN_XY[0]) / NUMBER_XYZ[0]; 
  theta[1]= static_cast <double>(SUCCESSES_IN_XY[1]) / NUMBER_XYZ[1]; 
	
  for(int i = 0; i < NUMBER_XYZ[0]; i++) {
    double u = gsl_ran_flat(gen, 0, 1);
    if(u < theta[0]) {
      successes[0]++;		
    }	
  }
  
  for(int i = 0; i < NUMBER_XYZ[1]; i++){
    double u = gsl_ran_flat(gen, 0,1);
    if(u < theta[1]){
      successes[1]++;		
    }	
  }	
  return successes;
}

void deletePointer(int* successes, double *returnValue)
{
  delete[]returnValue; 
  delete[]successes;
}

double* gibbsSampler(int successesZ, int* successesXY)
{
  double *returnValue = new double[2];	
  double theta;
  
  returnValue[0] = 0;
  returnValue[1] = 0;
	
  float  alpha = NUMBER_OF_STEPS_IN_MC + successesXY[0] + NUMBER_XYZ[1] - successesXY[1] + successesZ + 1;  	
  
  float beta  = NUMBER_XYZ[0] + NUMBER_XYZ[1] + NUMBER_XYZ[2] - NUMBER_OF_STEPS_IN_MC - successesXY[0] - successesZ +1;
	

  for(int i = 0; i < NUMBER_OF_STEPS_IN_MC; i++){
      alpha += TRANSITION_PROB_ALPHA[i];
      beta  += TRANSITION_PROB_BETA[i];
  }

  theta      = gsl_ran_beta(gen,alpha,beta);
  successesZ = gsl_ran_binomial(gen, theta, NUMBER_XYZ[2]);
	
  returnValue[0] = theta;
  returnValue[1] = successesZ;
		
  return returnValue;
}
