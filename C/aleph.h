#ifndef ALEPH_H_
#define ALEPH_H_

typedef unsigned long int u4;
 
// obtain a ALEPH pseudo-random value in [0..2**32]
u4  aleph_Random(void);
// seed ALEPH with a 256-bit block of 4-byte words (Bob Jenkins method) 
void aleph_SeedW(char *seed, int rounds);
// reset/initialize ALEPH
void aleph_Reset(void);
// ALEPH # of bits internal state
u4 aleph_StateBits(void);
// ALEPH expected cycle length
u4 aleph_Cycle(void);
// ALEPH maximum key length (bits)
u4 aleph_KeyLength(void);
// ALEPH Name
char* aleph_Name(void);

#endif
