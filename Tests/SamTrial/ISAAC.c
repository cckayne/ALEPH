#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "cktype.h"
#include "ISAAC.h"

/* are we testing? */
//#define TEST
/* verbose test output */
//#define VERBOSE
/* byte size of state array */
#define STSZ 256
#define STM1 STSZ-1
/*
------------------------------------------------------------------------------
My random number generator, ISAAC.
(c) Bob Jenkins, March 1996, Public Domain
You may use this code in any way you wish, and it is free.  No warrantee.
------------------------------------------------------------------------------
*/

/* external results */
ub4 rsl[STSZ], randcnt;

/* internal state */
static    ub4 mm[STSZ];
static    ub4 aa=0, bb=0, cc=0;


void ISAAC()
{
   register ub4 i,x,y;

   cc = cc + 1;    /* cc just gets incremented once per STSZ results */
   bb = bb + cc;   /* then combined with bb */

   for (i=0; i<STSZ; ++i)
   {
     x = mm[i];
     switch (i&3)
     {
     case 0: aa = aa^(aa<<13); break;
     case 1: aa = aa^(aa>>6); break;
     case 2: aa = aa^(aa<<2); break;
     case 3: aa = aa^(aa>>16); break;
     }
     aa = mm[(i+128)&STM1] + aa;
     mm[i] = y  = mm[(x>>2)&STM1] + aa + bb;
     rsl[i] = bb = mm[(y>>10)&STM1] + x;
   }
   // not in original readable.c
   randcnt = 0;
}

/* if (flag!=0), then use the contents of rsl[] to initialize mm[]. */
#define mix(a,b,c,d,e,f,g,h) \
{ \
   a^=b<<11; d+=a; b+=c; \
   b^=c>>2;  e+=b; c+=d; \
   c^=d<<8;  f+=c; d+=e; \
   d^=e>>16; g+=d; e+=f; \
   e^=f<<10; h+=e; f+=g; \
   f^=g>>4;  a+=f; g+=h; \
   g^=h<<8;  b+=g; h+=a; \
   h^=a>>9;  c+=h; a+=b; \
}

void ISAAC_init(int flag)
{
   register int i;
   ub4 a,b,c,d,e,f,g,h;
   aa=bb=cc=0;
   a=b=c=d=e=f=g=h=0x9e3779b9;  /* the golden ratio */

   for (i=0; i<4; ++i)          /* scramble it */
   {
     mix(a,b,c,d,e,f,g,h);
   }

   for (i=0; i<STSZ; i+=8)   /* fill in mm[] with messy stuff */
   {
     if (flag)                  /* use all the information in the seed */
	 {
       a+=rsl[i  ]; b+=rsl[i+1]; c+=rsl[i+2]; d+=rsl[i+3];
       e+=rsl[i+4]; f+=rsl[i+5]; g+=rsl[i+6]; h+=rsl[i+7];
     }
     mix(a,b,c,d,e,f,g,h);
     mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
     mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
   }

   if (flag)
   {        /* do a second pass to make all of the seed affect all of mm */
	 for (i=0; i<STSZ; i+=8)
     {
       a+=mm[i  ]; b+=mm[i+1]; c+=mm[i+2]; d+=mm[i+3];
       e+=mm[i+4]; f+=mm[i+5]; g+=mm[i+6]; h+=mm[i+7];
       mix(a,b,c,d,e,f,g,h);
       mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
       mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
     }
   }

   ISAAC();            /* fill in the first set of results */
   randcnt=0;        /* prepare to use the first set of results */
}


// Reset ISAAC's state
void ISAAC_Reset(void) {
	register ub4 i;
	aa=0; bb=0; cc=0; randcnt=0;
	for (i=0; i<STSZ; i++) { mm[i]=0; rsl[i]=0; }
}


// Get a random 32-bit value [0..MAXINT]
ub4 ISAAC_Random(void)
{
	ub4 r = rsl[randcnt];
	++randcnt;
	if (randcnt==STSZ) {
		ISAAC();
		randcnt = 0;
	}
	return r;
}


// Seed ISAAC with a 1024-bit block of 4-byte words (Bob Jenkins method) 
void ISAAC_SeedW(char *seed, int flag)
{
	register ub4 i,l;
	char s[MAXK];
	l=strlen(seed);
	memset(s,0,l+1);
	strcpy(s,seed);
	for (i=0; i<STSZ; i++) { mm[i]=0; rsl[i]=0; }
	memcpy((char *)rsl, (char *)s, l);
	ISAAC_init(flag);
}

