#ifndef MOTE64_H_
#define MOTE64_H_

typedef unsigned long int u4;
 
// obtain a MOTE pseudo-random value in [0..2**32]
u4  mote64_Random(void);
// seed MOTE with a 2048-bit block of 4-byte words (Bob Jenkins method) 
void mote64_SeedW(char *seed, int rounds);
// reset/initialize MOTE
void mote64_Reset(void);
// MOTE64 # of bits internal state
u4 mote64_StateBits(void);
// MOTE64 expected cycle length
u4 mote64_Cycle(void);
// MOTE64 maximum key length (bits)
u4 mote64_KeyLength(void);
// MOTE64 Name
char* mote64_Name(void);

#endif
