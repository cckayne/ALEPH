/* IOTA - A small-state CSPRNG and Stream Cipher
   IOTA is a IOTA with an 8+4-word internal state
   IOTA may be seeded with a 256-bit key
   IOTA Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
   IOTA is inspired by Bob Jenkins' PRNG insights (Public Domain).
*/
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "iota.h"

//#define TEST
//#define VERBOSE

// IOTA defines
/* internal state parameters */

//#define IOTA8
//#define IOTA16
#define IOTA32
//#define IOTA64

#ifdef IOTA8
#define STSZ 8
#define NAME "IOTA8"
#endif
#ifdef IOTA16
#define STSZ 16
#define NAME "IOTA16"
#endif
#ifdef IOTA32
#define STSZ 32
#define NAME "IOTA32"
#endif
#ifdef IOTA64
#define STSZ 64
#define NAME "IOTA64"
#endif
#define STM1 STSZ-1
#define STBYTES STSZ*4
#define STBITS 128+STBYTES*8

/* 2**32/phi, where phi is the golden ratio */
#define GOLDEN   0x9e3779b9
/* Fibonacci sequence */
#define IOTA ((bb=aa+bb),(aa=bb-aa))

static u4 state[STSZ], rsl[STSZ], a, b, c, d, e;
static u4 aa, bb, rcnt=0;

#ifdef TEST
static void statepeek(void);
#endif


static void iota(void) {
	u4 i;
	for (i=0; i<STSZ; i++) {
		e = state[d & STM1] + IOTA;
		state[d & STM1] = b ^ c;
		b = c - d;
		c = d + e;
		rsl[i] = d = e + state[i];
	}
	#ifdef TEST
	statepeek();
	#endif
}


// obtain a IOTA pseudo-random value in [0..2**32]
u4 iota_Random(void) {
	u4 r = rsl[rcnt];
	++rcnt;
	if (rcnt==STSZ) {
		iota();
		rcnt = 0;
	}
	return r;
}


void iota_Reset(void) {
	register u4 i,r;
	rcnt = 0;
	aa = bb = b = c = d = e = GOLDEN;
	for (i=0; i<STSZ; i++) { state[i]=GOLDEN; rsl[i]=0; }
}



// seed IOTA with a 256-bit block of 4-byte words (Bob Jenkins method) 
void iota_SeedW(char *seed, int rounds)
{
	register u4 i,l;
	char s[STBYTES*2];
	l=strlen(seed);
	if (l>STBYTES) l=STBYTES;
	memset(s,0,l+1);
	/* truncate seed to state-size if necessary */
	for (i=0; i<l; i++) s[i] = seed[i];
	iota_Reset();
	memcpy((char *)state, (char *)s, l);
	aa = state[0]; bb = state[1];
	iota();
	for (i=0; i<rounds; i++) iota_Random();  
}


// IOTA # of bits internal state
u4 iota_StateBits(void) {
	return STBITS;
}


// IOTA expected cycle length
u4 iota_Cycle(void) {
	return (STBITS+1)/2;
}


// IOTA maximum key length (bits)
u4 iota_KeyLength(void) {
	return STBYTES*8;
}


// IOTA Name
char* iota_Name(void) {
	return NAME;
}


#ifdef TEST
static u4 bcnt=0;
void testinit(u4 val) {
	register u4 i;
	rcnt = 0;
	b = c = d = e = val;
	for (i=0; i<STSZ; i++) { state[i]=val; rsl[i]=0; }
}
static void statepeek(void) {
	register u4 i;
	++bcnt;
	printf("%3u) iota%u...\n",bcnt,STSZ);
	for (i=0; i<STSZ; i++) {
		#ifdef VERBOSE
		printf("state %3u: %11u %c %02X  | rsl %3u: %11u %c %02X\n",
			i,state[i],state[i] % 26 + 'A',state[i] & 255,
			i,rsl[i],rsl[i] % 26 + 'A',rsl[i] & 255);
		#endif
	}
	printf("       b = %11u %c %02X\n       c = %11u %c %02X\n       d = %11u %c %02X\n       e = %11u %c %02X\n",
		b,b % 26+'A',b & 255,c,c % 26+'A',c & 255,d,d % 26+'A',d & 255,e,e % 26+'A',e & 255);
}
#endif
