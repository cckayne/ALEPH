/* ALEPH - A small-state CSPRNG and Stream Cipher
   ALEPH is a ALEPH with an 8+5-word internal state
   ALEPH may be seeded with a 256-bit key
   ALEPH Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
   ALEPH is inspired by Bob Jenkins' PRNG insights (Public Domain).
*/
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "aleph.h"

#define TEST
#define VERBOSE

// ALEPH defines

//#define ALEPH8
#define ALEPH16
//#define ALEPH32
//#define ALEPH64

/* internal state parameters */
#ifdef ALEPH8
#define STSZ 8
#define NAME "ALEPH8"
#endif
#ifdef ALEPH16
#define STSZ 16
#define NAME "ALEPH16"
#endif
#ifdef ALEPH32
#define STSZ 32
#define NAME "ALEPH32"
#endif
#ifdef ALEPH64
#define STSZ 64
#define NAME "ALEPH64"
#endif
#define STM1 STSZ-1
#define STBYTES STSZ*4
#define STBITS 128+STBYTES*8

/* 2**32/phi, where phi is the golden ratio */
#define GOLDEN   0x9e3779b9
/* indirection counter */
#define ALEPH (++a)

static u4 state[STSZ], rsl[STSZ], a, b, c, d, e, rcnt=0;

#ifdef TEST
static void statepeek(void);
#endif


static void aleph(void) {
	u4 i;
	for (i=0; i<STSZ; i++) {
		e = state[d & STM1] + ALEPH;
		state[d & STM1] = b ^ c;
		b = c - d;
		c = d + e;
		rsl[i] = d = e + state[i];
	}
	#ifdef TEST
	statepeek();
	#endif
}


// obtain a ALEPH pseudo-random value in [0..2**32]
u4 aleph_Random(void) {
	u4 r = rsl[rcnt];
	++rcnt;
	if (rcnt==STSZ) {
		aleph();
		rcnt = 0;
	}
	return r;
}


void aleph_Reset(void) {
	register u4 i,r;
	rcnt = 0;
	a = b = c = d = e = GOLDEN;
	for (i=0; i<STSZ; i++) { state[i]=GOLDEN; rsl[i]=GOLDEN; }
}


// seed ALEPH with a 2048-bit block of 4-byte words (Bob Jenkins method) 
void aleph_SeedW(char *seed, int rounds)
{
	register u4 i,l;
	char s[STBYTES*2];
	l=strlen(seed);
	if (l>STBYTES) l=STBYTES;
	memset(s,0,l+1);
	/* truncate seed to state-size if necessary */
	for (i=0; i<l; i++) s[i] = seed[i];
	aleph_Reset();
	memcpy((char *)state, (char *)s, l);
	/* fatten the variables on some seed-bytes */
	a+=state[3]; b+=state[2]; c+=state[1]; d+=state[0]; 
	aleph();
	for (i=0; i<rounds; i++) aleph_Random();  
}


// ALEPH # of bits internal state
u4 aleph_StateBits(void) {
	return STBITS;
}


// ALEPH expected cycle length
u4 aleph_Cycle(void) {
	return (STBITS+1)/2;
}


// ALEPH maximum key length (bits)
u4 aleph_KeyLength(void) {
	return STBYTES*8;
}


// ALEPH Name
char* aleph_Name(void) {
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
	printf("%3u) aleph%u...\n",bcnt,STSZ);
	for (i=0; i<STSZ; i++) {
		#ifdef VERBOSE
		printf("state %3u: %11u %c %02X  | rsl %3u: %11u %c %02X\n",
			i,state[i],state[i] % 26 + 'A',state[i] & 255,
			i,rsl[i],rsl[i] % 26 + 'A',rsl[i] & 255);
		#endif
	}
	printf("       a = %11u %c %02X\n       b = %11u %c %02X\n       c = %11u %c %02X\n       d = %11u %c %02X\n       e = %11u %c %02X\n",
		a,a % 26+'A',a & 255,b,b % 26+'A',b & 255,c,c % 26+'A',c & 255,d,d % 26+'A',d & 255,e,e % 26+'A',e & 255);
}
#endif
