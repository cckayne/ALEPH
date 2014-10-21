#ifndef IOTA_H_
#define IOTA_H_

typedef unsigned long int u4;
 
// obtain a IOTA pseudo-random value in [0..2**32]
u4  iota_Random(void);
// seed IOTA with a 256-bit block of 4-byte words (Bob Jenkins method) 
void iota_SeedW(char *seed, int rounds);
// reset/initialize IOTA
void iota_Reset(void);
// IOTA # of bits internal state
u4 iota_StateBits(void);
// IOTA expected cycle length
u4 iota_Cycle(void);
// IOTA maximum key length (bits)
u4 iota_KeyLength(void);
// IOTA Name
char* iota_Name(void);

#endif
