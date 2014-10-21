// CKTYPES  Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com  
#ifndef CKTYPE_H_   /* Include guard */
#define CKTYPE_H_

//#define DISTRIB
#define TRUE  1
#define FALSE 0
// terminal line length
#define LINE 70
// are we testing?
//#define TEST
// verbose test output
//#define VERBOSE
// are we logging?
#define LOG
// maximum text length (bytes)
#define MAXM 20480
#define MAXK 1025
// minimum text length (bytes)
#define MINM 2
#ifdef DISTRIB
	#define MINK 8
#else
	#define MINK 3
#endif
// nonce/IV lengths
#define NLEN 16
#define NLENMAX 48
#define NLENMIN 12
// maximum scrambler depth
#define MAXDEPTH 1000000 
// cipher modes for Caesar
enum ciphermode {
	cmEncipher, cmDecipher, cmNone 
};
// cipher types: Caesar, Caesar mixed, or Vernam
enum ciphertype {
	ctCaesar, ctCaesarM, ctVernam, ctNone 
};
// output format: ASCII or hex
enum outputform {
	ofASC, ofHEX, ofNone 
};
// a ub4 is an unsigned 4-byte quantity
typedef  unsigned long int  ub4;
// a ub8 is an unsigned 8-byte quantity
typedef unsigned long long ub8;

extern char MOD;
extern char START;
extern int SCRAMBLER;
extern int MIX;
extern int NONCE;
extern char* VERSION;

#endif
