#ifndef ISAAC_H_
#define ISAAC_H_

#include "cktype.h"

// Get a random 32-bit value [0..MAXINT]
ub4  ISAAC_Random();
// Seed ISAAC with a 1024-bit block of 4-byte words (Bob Jenkins method) 
void ISAAC_SeedW(char *seed, int flag);
// Reset ISAAC's state
void ISAAC_Reset(void);

#endif
