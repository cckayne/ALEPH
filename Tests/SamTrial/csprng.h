#ifndef CSPRNG_H_
#define CSPRNG_H_

#include "cktype.h"

// available CSPRNGs
enum CSPRNG { ISAAC, IOTA, MOTE8, MOTE16, MOTE32, MOTE64, BB128, BB256, BB512 };

// obtain a pseudo-random unsigned 32-bit quantity
//  from an available CSPRNG 
ub4  rRandom(enum CSPRNG rng);
// Initialize and seed a given CSPRNG
void rSeed(enum CSPRNG rng, char* seed, ub4 rounds);
// Initialize and seed available CSPRNGs
void rSeedAll(char* seed, ub4 rounds);
// Reset/initialize a given CSPRNG
void rReset(enum CSPRNG rng);
// Reset/initialize available CSPRNGs
void rResetAll(void);
// obtain the size of the rng's internal state array
ub4 rStateSize(enum CSPRNG rng);

#endif
