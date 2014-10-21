/* MOTE16 - A small-state CSPRNG and Stream Cipher
   MOTE16 is a MOTE with a 16+4-word internal state
   MOTE16 may be seeded with a 512-bit key
   MOTE16 Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
   MOTE16 is inspired by Bob Jenkins' PRNG insights (Public Domain).
*/
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "mote16.h"

//#define TEST
//#define VERBOSE

// MOTE defines
/* MOTE16N uses one's complement of e */
//#define NOT
#ifdef  NOT
#define NAME "MOTE16N"
#else
#define NAME "MOTE16"
#endif
/* internal state parameters */
#define STSZ 16
#define STM1 STSZ-1
#define STBYTES STSZ*4
#define STBITS 128+STBYTES*8

/* 2**32/phi, where phi is the golden ratio */
#define GOLDEN   0x9e3779b9
#define FLEASEED 0xf1ea5eed
#define rot(x,k) ((x<<k)|(x>>(32-k)))

static u4 rcnt=0;
static u4 state[STSZ], rsl[STSZ], b, c, d, e;

// MOTE16 ROT switcher
typedef struct Rsw { u4 iii; u4 jjj; u4 kkk; };
static u4 ri=0;
static const struct Rsw rsw[4] = {
29, 12, 16, // avalanche: 20.50 bits - 10/10 10/10 10/10; 2C:10/10
26, 16, 20, // avalanche: 19.50 bits - 10/10 10/10 10/10; 2C:10/10
15, 18, 12, // avalanche: 19.00 bits - 7/10 9/10 10/10; 3C:10/10
20, 20, 16  // avalanche: 19.00 bits - 4C:9/10,9/10,9/10,9/10,8/10,9/10,10/10
};

#ifdef TEST
static void statepeek(void);
#endif


// MOTE16 is refilled every 8 rounds
static void mote16(void) {
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
u4 mote16_Random(void) {
	u4 r = rsl[rcnt];
	++rcnt;
	if (rcnt==STSZ) {
		mote16();
		rcnt = 0;
	}
	return r;
}


void mote16_Reset(void) {
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


// seed MOTE with a 512-bit block of 4-byte words (Bob Jenkins method) 
void mote16_SeedW(char *seed, int rounds)
{
	register u4 i,l;
	char s[STBYTES*2];
	l=strlen(seed);
	if (l>STBYTES) l=STBYTES;
	memset(s,0,l+1);
	strcpy(s,seed);
	mote16_Reset();
	memcpy((char *)state, (char *)s, l);
	mix();
	mote16();
	for (i=0; i<rounds; i++) mote16_Random();  
}


// MOTE16 # of bits internal state
u4 mote16_StateBits(void) {
	return STBITS;
}


// MOTE16 expected cycle length
u4 mote16_Cycle(void) {
	return (STBITS+1)/2;
}


// MOTE16 maximum key length (bits)
u4 mote16_KeyLength(void) {
	return STBYTES*8;
}


// MOTE16 Name
char* mote16_Name(void) {
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
	printf("%3u) mote16 using rsw[%1u]...\n",bcnt,ri);
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
