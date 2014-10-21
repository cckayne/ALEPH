#ifndef MOTE16_H_
#define MOTE16_H_

typedef unsigned long int u4;
 
// obtain a MOTE pseudo-random value in [0..2**32]
u4  mote16_Random(void);
// seed MOTE with a 512-bit block of 4-byte words (Bob Jenkins method) 
void mote16_SeedW(char *seed, int rounds);
// reset/initialize MOTE
void mote16_Reset(void);
// MOTE16 # of bits internal state
u4 mote16_StateBits(void);
// MOTE16 expected cycle length
u4 mote16_Cycle(void);
// MOTE16 maximum key length (bits)
u4 mote16_KeyLength(void);
// MOTE16 Name
char* mote16_Name(void);

#endif
