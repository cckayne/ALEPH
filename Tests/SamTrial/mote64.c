/* MOTE64 - A small-state CSPRNG and Stream Cipher
   MOTE64 is a MOTE with a 64+4-word internal state
   MOTE64 may be seeded with a 2048-bit key
   MOTE64 Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
   MOTE64 is inspired by Bob Jenkins' PRNG insights (Public Domain).
*/
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "mote64.h"

// MOTE defines
//#define TEST
//#define VERBOSE
#define NAME "MOTE64"
#define STSZ 64
#define STM1 STSZ-1
#define STBYTES STSZ*4
#define STBITS 128+STBYTES*8

/* 2**32/phi, where phi is the golden ratio */
#define GOLDEN   0x9e3779b9
#define FLEASEED 0xf1ea5eed
#define rot(x,k) ((x<<k)|(x>>(32-k)))

static u4 rcnt=0;
static u4 state[STSZ], rsl[STSZ], b, c, d, e;

// MOTE64 ROT switcher
typedef struct Rsw { u4 iii; u4 jjj; u4 kkk; };
static u4 ri=0;
static const struct Rsw rsw[4] = {
11, 12, 24, // avalanche: 18.25 bits
 9,  8,  8, // avalanche: 18.00 bits
18, 20, 22, // avalanche: 17.88 bits
 3, 23, 30, // avalanche: 17.63 bits
};

#ifdef TEST
static void statepeek(void);
#endif


// MOTE64 is refilled every 64 rounds
static void mote64(void) {
	register u4 i;
	for (i=0; i<STSZ; i++) {
		state[c & STM1] = e;
		b = c ^ (e >> rsw[ri].iii | d << rsw[ri].jjj);
		c = d - rot(b,rsw[ri].kkk);
		d = state[i] + b;
		e = c + d;
		rsl[i] = c;
	}
	#ifdef TEST
	statepeek();
	#endif
	ri = d & 3;
}


// obtain a MOTE pseudo-random value in [0..2**32]
u4 mote64_Random(void) {
	u4 r = rsl[rcnt];
	++rcnt;
	if (rcnt==STSZ) {
		mote64();
		rcnt = 0;
	}
	return r;
}


void mote64_Reset(void) {
	register u4 i,r;
	rcnt = 0; ri = 0;
	b = c = d = e = FLEASEED;
	for (i=0; i<STSZ; i++) { state[i]=GOLDEN; rsl[i]=0; }
}


// mix in all the key and state bytes
static void mix(void) {
	register u4 i;
	for (i=0; i<STSZ; i++) {
		b = c ^ (e >> rsw[ri].iii | d << rsw[ri].jjj);
		c = d - rot(b,rsw[ri].kkk);
		d = state[i] + b;
		e = c + d;
		state[i] = e;
	}
}


// seed MOTE with a 2048-bit block of 4-byte words (Bob Jenkins method) 
void mote64_SeedW(char *seed, int rounds)
{
	register u4 i,l;
	char s[STBYTES*2];
	l=strlen(seed);
	if (l>STBYTES) l=STBYTES;
	memset(s,0,l+1);
	/* truncate seed to state-size if necessary */
	for (i=0; i<l; i++) s[i] = seed[i];
	mote64_Reset();
	memcpy((char *)state, (char *)s, l);
	mix();
	mote64();
	for (i=0; i<rounds; i++) mote64_Random();  
}


// MOTE64 # of bits internal state
u4 mote64_StateBits(void) {
	return STBITS;
}


// MOTE64 expected cycle length
u4 mote64_Cycle(void) {
	return (STBITS+1)/2;
}


// MOTE64 maximum key length (bits)
u4 mote64_KeyLength(void) {
	return STBYTES*8;
}


// MOTE64 Name
char* mote64_Name(void) {
	return NAME;
}


#ifdef TEST
static u4 bcnt=0;
static void statepeek(void) {
	register u4 i;
	++bcnt;
	printf("%3u) mote64 using rsw[%1u]...\n",bcnt,ri);
	for (i=0; i<STSZ; i++) {
		#ifdef VERBOSE
		printf("rsl %3u: %11u %c %02X  | state %3u: %11u %c %02X\n",
			i,rsl[i],rsl[i] % 26 + 'A',rsl[i] & 255,
			i,state[i],state[i] % 26 + 'A',state[i] & 255);
		#endif
	}
	printf("     b = %11u %c %02X\n     c = %11u %c %02X\n     d = %11u %c %02X\n     e = %11u %c %02X\n",
		b,b % 26+'A',b & 255,c,c % 26+'A',c & 255,d,d % 26+'A',d & 255,e,e % 26+'A',e & 255);
}
#endif
