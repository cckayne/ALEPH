#ifndef MOTE32_H_
#define MOTE32_H_

typedef unsigned long int u4;
 
// obtain a MOTE pseudo-random value in [0..2**32]
u4  mote32_Random(void);
// seed MOTE with a 1024-bit block of 4-byte words (Bob Jenkins method) 
void mote32_SeedW(char *seed, int rounds);
// reset/initialize MOTE
void mote32_Reset(void);
// MOTE32 # of bits internal state
u4 mote32_StateBits(void);
// MOTE32 expected cycle length
u4 mote32_Cycle(void);
// MOTE32 maximum key length (bits)
u4 mote32_KeyLength(void);
// MOTE32 Name
char* mote32_Name(void);

#endif
