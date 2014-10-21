unit mote32;
{  MOTE32 - A small-state CSPRNG and Stream Cipher
   MOTE32 is a MOTE with a 32+4-word internal state
   MOTE32 may be seeded with a 1024-bit key
   MOTE32 Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
   MOTE32 is inspired by Bob Jenkins' PRNG insights (Public Domain). }
{$mode delphi}
{$inline on}
//are we testing?
{ $define TEST}
//verbose test output
{ $define VERBOSE}

interface

// initialize/reset MOTE32
procedure moReset;
// obtain a MOTE32 pseudo-random value in [0..2**32]
function moRandom: Cardinal;
// seed MOTE32 with a 1024-bit block of 4-byte words (Bob Jenkins method) 
procedure moSeedW(seed: string; rounds: integer);
// MOTE32 # of bits internal state
function moStateBits: Cardinal;
// MOTE32 expected cycle length
function moCycle: Cardinal;
// MOTE32 Name
function moName: string;

implementation
{$ifdef TEST}uses StrUtils;{$endif}

const 	NAME = 'MOTE32';
		STSZ = 32;
		STM1 = STSZ-1;
		STBYTES = STSZ*4;
		STBITS = 128+STBYTES*8;
		// 2**32/phi, where phi is the golden ratio
		GOLDEN   = $9e3779b9;
		FLEASEED = $f1ea5eed;

		MODU = 26;
		START= 65;
		
// MOTE32 STATE
var		rsl: 	array[0..STSZ] of Cardinal;
		state: 	array[0..STSZ] of Cardinal;
		b,c,d,e,rcnt: Cardinal;

// MOTE32 ROT switcher
var			ri: Cardinal = 0;
type 		TRsw = record iii: Cardinal; jjj: Cardinal; kkk: Cardinal end;
type 		ARsw = array[0..3] of TRsw;
const		rsw: ARsw = ( 
(iii:30;  jjj: 4; kkk:20;), // avalanche: 19.00 bits
(iii:11;  jjj: 2; kkk:26;), // avalanche: 18.75 bits
(iii:19;  jjj:19; kkk:14;), // avalanche: 18.75 bits
(iii: 6;  jjj:16; kkk:12;)  // avalanche: 18.25 bits
);

	
function rot(var x: Cardinal; const k: Cardinal): Cardinal; inline;
	begin
		rot := (x shl k) or (x shr (32-k));
	end;

	
{$ifdef TEST}
var bcnt: Cardinal = 0;
procedure statepeek; 
	var i: Cardinal;
	begin
		inc(bcnt);
		Writeln(bcnt:3,') mote32 using rsw[',ri,']...');
		for i:=0 to STM1 do begin
			{$ifdef VERBOSE}
			Writeln('rsl ',
				i:3,': ',rsl[i]:11,chr(rsl[i] mod MODU + START):2,
					dec2numb((rsl[i] and 255),2,16):3,'  | state ',
				i:3,': ',state[i]:11,chr(state[i] mod MODU + START):2,
					dec2numb((state[i] and 255),2,16):3);
			{$endif}
		end;
		Writeln('     b = ',b:11,chr(b mod MODU+START):2,dec2numb((b and 255),2,16):3);     
		Writeln('     c = ',c:11,chr(c mod MODU+START):2,dec2numb((c and 255),2,16):3);    
		Writeln('     d = ',d:11,chr(d mod MODU+START):2,dec2numb((d and 255),2,16):3);
		Writeln('     e = ',e:11,chr(e mod MODU+START):2,dec2numb((e and 255),2,16):3);
	end;
{$endif}


// MOTE32 is filled every 32 rounds
procedure mote32;
    var i: Cardinal;
	begin
		for i:=0 to STM1 do begin
			state[c and STM1] := e;
			b := c xor ((e shr rsw[ri].iii) or (d shl rsw[ri].jjj));
			c := d - rot(b,rsw[ri].kkk);
			d := state[i] + b;
			e := c + d;
			rsl[i] := c;
		end;
		{$ifdef TEST}
		statepeek;
		{$endif}
		ri := (d and 3);
	end;
	
	
// initialize/reset MOTE32 (obligatory)
procedure moReset;
	var i: Cardinal;
	begin
		rcnt:=0;
		b := FLEASEED; c := b; d:=c; e:=d;
		for i:=0 to STM1 do begin
			state[i] := GOLDEN; rsl[i] := 0;
		end;
	end;


// obtain a MOTE pseudo-random value in [0..2**32]
function moRandom: Cardinal;
	begin
		moRandom := rsl[rcnt];
		inc(rcnt);
		if (rcnt=STSZ) then begin
			mote32;
			rcnt := 0;
		end;
	end;


// mix in all the key and state bytes
procedure mix;
	var i: Cardinal;
	begin
		for i:=0 to STM1 do begin
			b := c xor ((e shr rsw[ri].iii) or (d shl rsw[ri].jjj));
			c := d - rot(b,rsw[ri].kkk);
			d := state[i] + b;
			e := c + d;
			state[i] := e;
		end;
	end;

	
// seed MOTE32 with a 1024-bit block of 4-byte words (Bob Jenkins method) 
procedure moSeedW(seed: string; rounds: integer);
	var i,l: Cardinal;
		p: pointer;
	begin
		p:=@state[0];
		l:=Length(seed);
		if (l>STBYTES) then l:=STBYTES;
		moReset;
		for i:=0 to l-1 do
			byte((p+i)^) := byte(seed[i+1]);
		mix;
		mote32;
		for i:=1 to rounds do moRandom;  
	end;


// MOTE32 # of bits internal state
function moStateBits: Cardinal;
	begin
		moStateBits := STBITS;
	end;


// MOTE32 expected cycle length
function moCycle: Cardinal;
	begin
		moCycle := (STBITS+1) div 2;
	end;


// MOTE32 Name
function moName: string;
	begin
		moName := NAME;
	end;

end.