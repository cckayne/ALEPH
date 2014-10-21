#ifndef MOTE8_H_
#define MOTE8_H_

typedef unsigned long int u4;
 
// obtain a MOTE pseudo-random value in [0..2**32]
u4  mote8_Random(void);
// seed MOTE with a 256-bit block of 4-byte words (Bob Jenkins method) 
void mote8_SeedW(char *seed, int rounds);
// reset/initialize MOTE
void mote8_Reset(void);
// MOTE8 # of bits internal state
u4 mote8_StateBits(void);
// MOTE8 expected cycle length
u4 mote8_Cycle(void);
// MOTE8 maximum key length (bits)
u4 mote8_KeyLength(void);
// MOTE8 Name
char* mote8_Name(void);

#endif
