/*  BEDBUG128 - A FLEA-inspired CSPRNG and Stream Cipher
    BEDBUG128 is a BEDBUG with a 128-byte internal state array
    BEDBUG128 may be seeded with a 512-bit key
    BEDBUG128 Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
    BEDBUG128 is based on FLEA and other PRNG insights by Bob Jenkins. Public Domain.
*/
#ifndef BB128_H_
#define BB128_H_

typedef unsigned long int ub4;
 
// obtain a BEDBUG128 pseudo-random value in [0..2**32]
ub4  bb128_Random(void);
// seed BEDBUG128 with a 512-bit block of 4-byte words (Bob Jenkins method) 
void bb128_SeedW(char *seed, int rounds);
// reset/initialize BEDBUG128
void bb128_Reset(void);

#endif
