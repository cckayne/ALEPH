// BEDBUG256 - A FLEA-inspired CSPRNG and Stream Cipher
// BEDBUG256 is a BEDBUG with a 256-byte internal state array
// BEDBUG256 may be seeded with a 1024-bit key
// BEDBUG256 Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
// BEDBUG256 is based on FLEA and other PRNG insights by Bob Jenkins. Public Domain.
#ifndef BB256_H_
#define BB256_H_

typedef unsigned long int ub4;
 
// obtain a BEDBUG256 pseudo-random value in [0..2**32]
ub4  bb256_Random(void);
// seed BEDBUG256 with a 1024-bit block of 4-byte words (Bob Jenkins method) 
void bb256_SeedW(char *seed, int rounds);
// reset/initialize BEDBUG256
void bb256_Reset(void);

#endif
