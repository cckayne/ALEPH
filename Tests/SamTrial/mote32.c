#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "mote32.h"

// MOTE defines
//#define TEST
//#define VERBOSE
#define NAME "MOTE32"
#define STSZ 32
#define STM1 STSZ-1
#define STBYTES STSZ*4
#define STBITS 128+STBYTES*8

/* 2**32/phi, where phi is the golden ratio */
#define GOLDEN   0x9e3779b9
#define FLEASEED 0xf1ea5eed
#define rot(x,k) ((x<<k)|(x>>(32-k)))

static u4 rcnt=0;
static u4 state[STSZ], rsl[STSZ], b, c, d, e;

// MOTE32 ROT switcher
typedef struct Rsw { u4 iii; u4 jjj; u4 kkk; };
static u4 ri=0;
static const struct Rsw rsw[4] = {
30,  4, 20, // avalanche: 19.00 bits
11,  2, 26, // avalanche: 18.75 bits
19, 19, 14, // avalanche: 18.75 bits
 6, 16, 12  // avalanche: 18.25 bits
};

#ifdef TEST
static void statepeek(void);
#endif


// MOTE32 is refilled every 32 rounds
static void mote32(void) {
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
u4 mote32_Random(void) {
	u4 r = rsl[rcnt];
	++rcnt;
	if (rcnt==STSZ) {
		mote32();
		rcnt = 0;
	}
	return r;
}


void mote32_Reset(void) {
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


// seed MOTE with a 1024-bit block of 4-byte words (Bob Jenkins method) 
void mote32_SeedW(char *seed, int rounds)
{
	register u4 i,l;
	char s[STBYTES*2];
	l=strlen(seed);
	if (l>STBYTES) l=STBYTES;
	memset(s,0,l+1);
	strcpy(s,seed);
	mote32_Reset();
	memcpy((char *)state, (char *)s, l);
	mix();
	mote32();
	for (i=0; i<rounds; i++) mote32_Random();  
}


// MOTE32 # of bits internal state
u4 mote32_StateBits(void) {
	return STBITS;
}


// MOTE32 expected cycle length
u4 mote32_Cycle(void) {
	return (STBITS+1)/2;
}


// MOTE32 maximum key length (bits)
u4 mote32_KeyLength(void) {
	return STBYTES*8;
}


// MOTE32 Name
char* mote32_Name(void) {
	return NAME;
}


#ifdef TEST
static u4 bcnt=0;
static void statepeek(void) {
	register u4 i;
	++bcnt;
	printf("%3u) mote32 using rsw[%1u]...\n",bcnt,ri);
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
