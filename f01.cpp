/* Includes */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* Definitions */
#define P_NUM 50 /* Particle number */
#define SPACE_DIM 2 /* Space dimension */
#define COMPUTATIONS 8000 /* Number of computation after which particles stop moving */
#define INERTIA 0.5 
#define C_SOC 1.9 /* Social component constant*/
#define C_COG 1.9 /* Cognitive component constant */
#define MAX_VEL 10.0 
#define F_BIAS -450 /* f_bias, as definition */

float shifted_origin[SPACE_DIM];

struct particle {
  float position[SPACE_DIM];
  float vel[SPACE_DIM];
  float personal_best[SPACE_DIM];
};

void particles_init(struct particle &p);

float f01(float position[]);

/* MAIN */
int main(int argc, char** argv) {
  
  srand(time(NULL));

  particle particles[P_NUM];
  
  float global_best[SPACE_DIM];

  // Set shifted origin to some place 
  for(int i=0; i<SPACE_DIM; ++i)
    shifted_origin[i] = 83; 
  
  // Init every particle
  for(int i=0; i<P_NUM; ++i)
    particles_init(particles[i]);
  
  for(int i=0; i<COMPUTATIONS; ++i) {
    for(int pos=0; pos<P_NUM; ++pos) {
      for(int direction=0; direction<SPACE_DIM; ++direction) {
	// Compute new position with damping if needed
	particles[pos].position[direction] = particles[pos].position[direction] + particles[pos].vel[direction];
	if(particles[pos].position[direction] < 100 || particles[pos].position[direction] > 100) {
	  float damping_rand = (float)rand() / (float)RAND_MAX;
	  particles[pos].vel[direction] = -damping_rand * particles[pos].vel[direction];
	}
	// Compute new velocity
	float r1 = (float)rand() / (float)RAND_MAX;
	float r2 = (float)rand() / (float)RAND_MAX;
	particles[pos].vel[direction] = INERTIA*particles[pos].vel[direction] + 
	  r1 * C_SOC * (global_best[direction] - particles[pos].position[direction]) +
	  r2 * C_SOC * (particles[pos].personal_best[direction] - particles[pos].position[direction]);

	// Limit velocity
	if(particles[pos].vel[direction] > MAX_VEL)
	  particles[pos].vel[direction] = MAX_VEL;
	else if(particles[pos].vel[direction] < -MAX_VEL)
	  particles[pos].vel[direction] = -MAX_VEL;

	// If needed, update personal and global best
	if (f01(particles[pos].position) < f01(particles[pos].personal_best)) 
	  for(int k=0; k<SPACE_DIM; ++k)
	    particles[pos].personal_best[k] = particles[pos].position[k];
	if (f01(particles[pos].position) < f01(global_best)) 
	  for(int k=0; k<SPACE_DIM; ++k)
	    global_best[k] = particles[pos].position[k];
      }
    }
  }
  std::cout << "Global best : " << f01(global_best) << std::endl;

  std::cout << "Global optimum : (";
  for (int i=0; i<SPACE_DIM; ++i) {
    std::cout << global_best[i];
    if(i!=SPACE_DIM-1)
      std::cout << ", ";
  }
  std::cout << ")" << std::endl;
  return 0;
}

void particles_init(struct particle &p) {
  for(int j=0; j<SPACE_DIM; ++j) {
    float random_pso = ( ( (float)rand() / (float)RAND_MAX ) * 201) - 101;
    p.position[j] = random_pso; // from -100 to +100, as defined
    p.vel[j] =0.0f;
    p.personal_best[j] = p.position[j];
  }
}

float f01(float position[]) {
  /* f_1 as defined */
  float score =0.0f;
  for (int i=0; i<SPACE_DIM; ++i)
    score += pow(position[i] - shifted_origin[i], 2);
  score += F_BIAS;
  return score;
}
