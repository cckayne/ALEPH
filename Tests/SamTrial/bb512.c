/*  BEDBUG512 - A FLEA-inspired CSPRNG and Stream Cipher
    BEDBUG512 is a BEDBUG with a 512-byte internal state array
    BEDBUG512 may be seeded with a 2048-bit key
    BEDBUG512 Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
    BEDBUG512 is based on FLEA and other PRNG insights by Bob Jenkins. Public Domain.
*/
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "bb512.h"

/* are we testing? */
//#define TEST
/* verbose test output */
//#define VERBOSE
/* byte size of state array */
#define STSZ 512
#define STM1 STSZ-1
/* BB512b performs an extra pseudo-random lookup */
#define B
/* BB512 option: switch ROT constants at each bb512() call */
#define RSW
/* 2**32/phi, where phi is the golden ratio */
#define GOLDEN 0x9e3779b9

// BB512 STATE
static ub4 rsl[STSZ], state[STSZ];
static ub4 b,c,d,e, rcnt=0;

// BB512 ROT switcher
typedef struct Rsw { ub4 iii; ub4 jjj; ub4 kkk; };
static ub4 ri=0;
static const struct Rsw rsw[4] = {
#ifdef B
17,  3,  4, // avalanche: 16.56 bits (worst case)
15, 18, 23, // avalanche: 16.52 bits (worst case)
 3, 23, 24, // avalanche: 16.52 bits (worst case)
 3, 15,  8  // avalanche: 16.53 bits (worst case)
#else
24,  4, 23, // avalanche: 16.80 bits (worst case)
18, 26,  2, // avalanche: 16.50 bits (worst case)
 3,  6, 22, // avalanche: 16.50 bits (worst case)
20, 17,  8  // avalanche: 16.66 bits (worst case)
#endif
};

#define rot(x,k) ((x<<k)|(x>>(32-k)))


#ifdef TEST
static ub4 bcnt=0;
static void statepeek(void) {
	register ub4 i, gcnt=0;
	++bcnt;
	printf("%3u) bb512 using rsw[%1u]...\n",bcnt,ri);
	for (i=0; i<STSZ; i++) {
		#ifdef VERBOSE
		printf("rsl %3u: %11u %c %02X  | state %3u: %11u %c %02X\n",
			i,rsl[i],rsl[i] % 26 + 'A',rsl[i] & 255,
			i,state[i],state[i] % 26 + 'A',state[i] & 255);
		#endif
		if (state[i]==GOLDEN) ++gcnt;
	}
	printf("     b = %11u %c %02X\n     c = %11u %c %02X\n     d = %11u %c %02X\n",
		b,b % 26+'A',b & 255,c,c % 26+'A',c & 255, d,d % 26+'A',d & 255);
	printf("     %u GOLDEN\n",gcnt);
}
#endif


// BEDBUG512 is filled every 512 rounds
static void bb512(void) {
    register ub4 i;
	for (i=0; i<STSZ; i++) {
		e = state[d & STM1] - rot(b,rsw[ri].iii);
		state[d & STM1] = b ^ rot(c,rsw[ri].jjj);
		b = c + rot(d,rsw[ri].kkk);
		c = d + e;
		#ifdef B
		d = e + state[b & STM1];
		#else
		d = e + state[i];
		#endif
		rsl[i] = d;
	}
	#ifdef TEST
	statepeek();
	#endif
	#ifdef RSW
	ri = (c & 3);
	#endif
}
	
	
// reset/initialize BEDBUG512 (obligatory)
void bb512_Reset(void) {
	register ub4 i;
	rcnt=0;
	b = c = d = e = GOLDEN;
	for (i=0; i<STSZ; i++) {
		state[i] = GOLDEN; rsl[i] = 0;
	}
}


// obtain a BEDBUG512 pseudo-random value in [0..2**32]
ub4 bb512_Random(void) {
	ub4 r = rsl[rcnt];
	++rcnt;
	if (rcnt==STSZ) {
		bb512();
		rcnt = 0;
	}
	return r;
}


// seed BEDBUG512 with a 2048-bit block of 4-byte words (Bob Jenkins method) 
void bb512_SeedW(char *seed, int rounds)
{
	register ub4 i,l;
	char s[STSZ];
	l=strlen(seed);
	memset(s,0,l+1);
	strcpy(s,seed);
	bb512_Reset();
	memcpy((char *)state, (char *)s, l);
	bb512();
	for (i=0; i<rounds; i++) bb512_Random();  
}
