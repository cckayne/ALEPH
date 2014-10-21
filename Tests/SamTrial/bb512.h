// BEDBUG512 - A FLEA-inspired CSPRNG and Stream Cipher
// BEDBUG512 is a BEDBUG with a 512-byte internal state array
// BEDBUG512 may be seeded with a 2048-bit key
// BEDBUG512 Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
// BEDBUG512 is based on FLEA and other PRNG insights by Bob Jenkins. Public Domain.
#ifndef BB512_H_
#define BB512_H_

typedef unsigned long int ub4;

// obtain a BEDBUG512 pseudo-random value in [0..2**32]
ub4  bb512_Random(void);
// seed BEDBUG512 with a 1024-bit block of 4-byte words (Bob Jenkins method) 
void bb512_SeedW(char *seed, int rounds);
// reset/initialize BEDBUG512
void bb512_Reset(void);

#endif
