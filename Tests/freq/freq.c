/*
 * Measure whether all values appear with equal frequency
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

typedef  unsigned char      u1;
typedef  unsigned long      u4;
typedef  unsigned long long u8;

#define BUCKETS (1<<8)

#define ALEPH16
//#define ALEPH32
//#define ALEPH64

#ifdef ALEPH8
#define STSZ 8
#endif
#ifdef ALEPH16
#define STSZ 16
#endif
#ifdef ALEPH32
#define STSZ 32
#endif
#ifdef ALEPH64
#define STSZ 64
#endif
#define STM1 STSZ-1

/* 2**32/phi, where phi is the golden ratio */
#define GOLDEN   0x9e3779b9

typedef struct ranctx { u4 state[STSZ]; u4 rsl[STSZ]; u4 a; u4 b; u4 c; u4 d; u4 e;} ranctx;
static u4 rcnt=0; static u8 fcount=0; static u4 icount=0;

#define ALEPH (++x->a)

static u4 ranval(ranctx *x) {
	u4 i,r;
	r = x->rsl[rcnt];
	++rcnt; 
	#ifdef VERBOSE
	++fcount;
	#endif
	if (rcnt==STSZ) {
		for (i=0; i<STSZ; i++) {
			x->e = x->state[x->d & STM1] + ALEPH;
			x->state[x->d & STM1] = x->b ^ x->c;
			x->b = x->c - x->d;
			x->c = x->d + x->e;
			x->d = x->e + x->state[i];
			x->rsl[i] = x->d;
		}
		rcnt = 0;
	}
	return r;
}


static void raninit(ranctx *x, u4 seed) {
	register u4 i,r;
	icount++;
	x->a = x->b = x->c = x->d = GOLDEN;
	for (i=0; i<STSZ; i++) { x->state[i]=GOLDEN; x->rsl[i]=0; }
	x->state[0] = seed;
	// init with u4 seed
	for (i=0; i<20; i++) r = ranval(x);
}


/* initialize the data collection array */
static void datainit( u8 *data)
{
  u4 i;
  for (i=0; i<BUCKETS; ++i) data[i] = (u8)0;
}

/* gather statistics on len overlapping subsequences of length 5 each */
#define INCREMENT 0x10000
static void gather( ranctx *r, u8 *data, u8 len)
{
  u8 i;
  u4 j;
  for (i=0; i<len; i+=INCREMENT) {
    for (j=0; j<INCREMENT; j+=4) {
      ++data[ranval(r)&(BUCKETS-1)];
      ++data[ranval(r)&(BUCKETS-1)];
      ++data[ranval(r)&(BUCKETS-1)];
      ++data[ranval(r)&(BUCKETS-1)];
    }
  }
}

static void chi( u8 *data, u8 len)
{
  u4 i;
  double var = 0.0;         /* total variance */
  double temp;              /* used to calculate variance of a bucket */
  double expect = ((double)len)/BUCKETS;
  
  for (i=0; i<BUCKETS; ++i) {
    double temp = (double)data[i] - expect;
    var += temp*temp/expect;
  }

  /* calculate the total variance and chi-square measure */
  printf("expected variance: %11.4f   got: %11.4f   chi-square: %6.4f\n",
         (float)(BUCKETS-1), (float)var, 
	 (float)((var-(BUCKETS-1))/sqrt((float)(BUCKETS-1))));
}


int main( int argc, char **argv)
{
  u8 len;
  u8 data[BUCKETS];
  u4 loglen = 0;
  ranctx r;
  time_t a,z;
  
  #ifdef ALEPH8
  printf("Testing ALEPH8");
  #endif
  #ifdef ALEPH16
  printf("Testing ALEPH16");
  #endif
  #ifdef ALEPH32
  printf("Testing ALEPH32");
  #endif
  #ifdef ALEPH64
  printf("Testing ALEPH64");
  #endif
  puts(" ...");

  time(&a);
  if (argc == 2) {
    sscanf(argv[1], "%d", &loglen);
    printf("log_2 sequence length: %d\n", loglen);
    len = (((u8)1)<<loglen);
  } else {
    fprintf(stderr, "usage: \"freq 24\" means run for 2^^24 values\n");
    return 1;
  }

  datainit(data);
  raninit(&r, 0);
  gather(&r, data, len);
  chi(data, len);

  time(&z);
  printf("\nnumber of seconds      : %12d\n", (size_t)(z-a));
  printf("number of ranval calls : %12llu\n", fcount);
  printf("number of raninit calls: %12u\n", icount);
}
