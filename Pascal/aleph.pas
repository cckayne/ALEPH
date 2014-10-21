unit aleph;
(* ALEPH - A minimal CSPRNG reflecting the Zen of cipher design
   ALEPH is an ALEPH with a 16,32 or 64+4-word internal state
   ALEPH may be seeded with a 512-, 1024- or 2048-bit key
   ALEPH Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
   ALEPH is inspired by Bob Jenkins' PRNG insights (Public Domain).
*)
{$mode delphi}
{$inline on}
//are we testing?
{$define TEST}
//verbose test output
{$define VERBOSE}

interface

// initialize/reset ALEPH
procedure alReset;
// obtain an ALEPH pseudo-random value in [0..2**32]
function alRandom: Cardinal;
// seed ALEPH with a 2048-bit block of 4-byte words (Bob Jenkins method) 
procedure alSeedW(seed: string; rounds: integer);
// ALEPH # of bits internal state
function alStateBits: Cardinal;
// ALEPH expected cycle length
function alCycle: Cardinal;
// ALEPH Name
function alName: string;

implementation
{$ifdef TEST}uses StrUtils;{$endif}

// select your ALEPH variant here
{$define ALEPH16}
{ $define ALEPH32}
{ $define ALEPH64}

const 	
		{$ifdef ALEPH16}
		NAME = 'ALEPH16';
		STSZ = 16;
		{$endif}
		{$ifdef ALEPH32}
		NAME = 'ALEPH32';
		STSZ = 32;
		{$endif}
		{$ifdef ALEPH64}
		NAME = 'ALEPH64';
		STSZ = 64;
		{$endif}
		
		STM1 = STSZ-1;
		STBYTES = STSZ*4;
		STBITS = 160+STBYTES*8;
		// 2**32/phi, where phi is the golden ratio
		GOLDEN   = $9e3779b9;

		MODU = 26;
		START= 65;
		
// ALEPH STATE
var		rsl: 	array[0..STSZ] of Cardinal;
		state: 	array[0..STSZ] of Cardinal;
		a,b,c,d,e,rcnt: Cardinal;

	
{$ifdef TEST}
var bcnt: Cardinal = 0;
procedure statepeek; 
	var i: Cardinal;
	begin
		inc(bcnt);
		Writeln(bcnt:3,') aleph ...');
		for i:=0 to STM1 do begin
			{$ifdef VERBOSE}
			Writeln('state ',
				i:3,': ',state[i]:11,chr(state[i] mod MODU + START):2,
					dec2numb((state[i] and 255),2,16):3,'  | rsl ',
				i:3,': ',rsl[i]:11,chr(rsl[i] mod MODU + START):2,
					dec2numb((rsl[i] and 255),2,16):3);
			{$endif}
		end;
		Writeln('       a = ',a:11,chr(a mod MODU+START):2,dec2numb((a and 255),2,16):3);     
		Writeln('       b = ',b:11,chr(b mod MODU+START):2,dec2numb((b and 255),2,16):3);     
		Writeln('       c = ',c:11,chr(c mod MODU+START):2,dec2numb((c and 255),2,16):3);    
		Writeln('       d = ',d:11,chr(d mod MODU+START):2,dec2numb((d and 255),2,16):3);
		Writeln('       e = ',e:11,chr(e mod MODU+START):2,dec2numb((e and 255),2,16):3);
	end;
{$endif}


// ALEPH is filled every 16, 32 or 64 rounds
procedure aleph;
    var i: Cardinal;
	begin
		for i:=0 to STM1 do begin
			inc(a);
			e := state[d and STM1] + a;
			state[d and STM1] := b xor c;
			b := c - d;
			c := d + e;
			d := e + state[i];
			rsl[i] := d;			
		end;
		{$ifdef TEST}
		statepeek;
		{$endif}
	end;
	
	
// initialize/reset ALEPH (obligatory)
procedure alReset;
	var i: Cardinal;
	begin
		rcnt:=0;
		a := GOLDEN; b := a; c := b; d:=c; e:=d;
		for i:=0 to STM1 do begin
			state[i] := GOLDEN; rsl[i] := 0;
		end;
	end;


// obtain a ALEPH pseudo-random value in [0..2**32]
function alRandom: Cardinal;
	begin
		alRandom := rsl[rcnt];
		inc(rcnt);
		if (rcnt=STSZ) then begin
			aleph;
			rcnt := 0;
		end;
	end;


// seed ALEPH with a 2048-bit block of 4-byte words (Bob Jenkins method) 
procedure alSeedW(seed: string; rounds: integer);
	var i,l: Cardinal;
		p: pointer;
	begin
		p:=@state[0];
		l:=Length(seed);
		if (l>STBYTES) then l:=STBYTES;
		alReset;
		for i:=0 to l-1 do
			byte((p+i)^) := byte(seed[i+1]);
		// fatten the variables on some seed-bytes
		a+=state[3]; b+=state[2]; c+=state[1]; d+=state[0]; 
		aleph;
		for i:=1 to rounds do alRandom;  
	end;


// ALEPH # of bits internal state
function alStateBits: Cardinal;
	begin
		alStateBits := STBITS;
	end;


// ALEPH expected cycle length
function alCycle: Cardinal;
	begin
		alCycle := (STBITS+1) div 2;
	end;


// ALEPH Name
function alName: string;
	begin
		alName := NAME;
	end;

end.