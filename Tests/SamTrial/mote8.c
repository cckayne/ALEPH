/* MOTE8 - A small-state CSPRNG and Stream Cipher
   MOTE8 is a MOTE with an 8+4-word internal state
   MOTE8 may be seeded with a 256-bit key
   MOTE8 Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
   MOTE8 is inspired by Bob Jenkins' PRNG insights (Public Domain).
*/
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "mote8.h"

//#define TEST
//#define VERBOSE

// MOTE defines
#define NAME "MOTE8"
/* internal state parameters */
#define STSZ 8
#define STM1 STSZ-1
#define STBYTES STSZ*4
#define STBITS 128+STBYTES*8

/* 2**32/phi, where phi is the golden ratio */
#define GOLDEN   0x9e3779b9
#define FLEASEED 0xf1ea5eed
#define rot(x,k) ((x<<k)|(x>>(32-k)))

static u4 rcnt=0;
static u4 state[STSZ], rsl[STSZ], b, c, d, e;

// MOTE8 ROT switcher
typedef struct Rsw { u4 iii; u4 jjj; u4 kkk; };
static u4 ri=0;
static const struct Rsw rsw[4] = {
28,  19, 16, // avalanche: 21.0 bits (worst case)
17,  15, 28, // avalanche: 21.0 bits (worst case)
15,  28, 12, // avalanche: 21.0 bits (worst case)
25,   5, 20  // avalanche: 21.0 bits (worst case)
};

#ifdef TEST
static void statepeek(void);
#endif


// MOTE8 is refilled every 8 rounds
static void mote8(void) {
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
u4 mote8_Random(void) {
	u4 r = rsl[rcnt];
	++rcnt;
	if (rcnt==STSZ) {
		mote8();
		rcnt = 0;
	}
	return r;
}


void mote8_Reset(void) {
	register u4 i,r;
	rcnt = 0;
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


// seed MOTE with a 256-bit block of 4-byte words (Bob Jenkins method) 
void mote8_SeedW(char *seed, int rounds)
{
	register u4 i,l;
	char s[STBYTES*2];
	l=strlen(seed);
	if (l>STBYTES) l=STBYTES;
	memset(s,0,l+1);
	strcpy(s,seed);
	mote8_Reset();
	memcpy((char *)state, (char *)s, l);
	mix();
	mote8();
	for (i=0; i<rounds; i++) mote8_Random();  
}


// MOTE8 # of bits internal state
u4 mote8_StateBits(void) {
	return STBITS;
}


// MOTE8 expected cycle length
u4 mote8_Cycle(void) {
	return (STBITS+1)/2;
}


// MOTE8 maximum key length (bits)
u4 mote8_KeyLength(void) {
	return STBYTES*8;
}


// MOTE8 Name
char* mote8_Name(void) {
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
	printf("%3u) mote8 using rsw[%1u]...\n",bcnt,ri);
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
