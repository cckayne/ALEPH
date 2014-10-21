unit aum;
(* AUM - A small-state CSPRNG reflecting the Zen of cipher design
   AUM is an AUM with a 16,32 or 64+4-word internal state
   AUM may be seeded with a 512-, 1024- or 2048-bit key
   AUM Copyright C.C.Kayne 2014, GNU GPL V.3, cckayne@gmail.com
   AUM is inspired by Bob Jenkins' PRNG insights (Public Domain).
*)
{$mode delphi}
{$inline on}
//are we testing?
{ $define TEST}
//verbose test output
{ $define VERBOSE}

interface

// initialize/reset AUM
procedure auReset;
// obtain an AUM pseudo-random value in [0..2**32]
function auRandom: Cardinal;
// seed AUM with a 512-bit block of 4-byte words (Bob Jenkins method) 
procedure auSeedW(seed: string; rounds: integer);
// AUM # of bits internal state
function auStateBits: Cardinal;
// AUM expected cycle length
function auCycle: Cardinal;
// AUM Name
function auName: string;

implementation
{$ifdef TEST}uses StrUtils;{$endif}

{$define AUM16}
{ $define AUM32}
{ $define AUM64}

const 	
		{$ifdef AUM16}
		NAME = 'AUM16';
		STSZ = 16;
		{$endif}
		{$ifdef AUM32}
		NAME = 'AUM32';
		STSZ = 32;
		{$endif}
		{$ifdef AUM64}
		NAME = 'AUM64';
		STSZ = 64;
		{$endif}
		
		STM1 = STSZ-1;
		STBYTES = STSZ*4;
		STBITS = 128+STBYTES*8;
		// 2**32/phi, where phi is the golden ratio
		GOLDEN   = $9e3779b9;

		MODU = 26;
		START= 65;
		
// AUM STATE
var		rsl: 	array[0..STSZ] of Cardinal;
		state: 	array[0..STSZ] of Cardinal;
		b,c,d,e,rcnt: Cardinal;

	
{$ifdef TEST}
var bcnt: Cardinal = 0;
procedure statepeek; 
	var i: Cardinal;
	begin
		inc(bcnt);
		Writeln(bcnt:3,') aum ...');
		for i:=0 to STM1 do begin
			{$ifdef VERBOSE}
			Writeln('state ',
				i:3,': ',state[i]:11,chr(state[i] mod MODU + START):2,
					dec2numb((state[i] and 255),2,16):3,'  | rsl ',
				i:3,': ',rsl[i]:11,chr(rsl[i] mod MODU + START):2,
					dec2numb((rsl[i] and 255),2,16):3);
			{$endif}
		end;
		Writeln('       b = ',b:11,chr(b mod MODU+START):2,dec2numb((b and 255),2,16):3);     
		Writeln('       c = ',c:11,chr(c mod MODU+START):2,dec2numb((c and 255),2,16):3);    
		Writeln('       d = ',d:11,chr(d mod MODU+START):2,dec2numb((d and 255),2,16):3);
		Writeln('       e = ',e:11,chr(e mod MODU+START):2,dec2numb((e and 255),2,16):3);
	end;
{$endif}


// AUM is filled every 16, 32 or 64 rounds
procedure aum;
    var i: Cardinal;
	begin
		for i:=0 to STM1 do begin
			e := state[d and STM1];
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
	
	
// initialize/reset AUM (obligatory)
procedure auReset;
	var i: Cardinal;
	begin
		rcnt:=0;
		b := GOLDEN; c := b; d:=c; e:=d;
		for i:=0 to STM1 do begin
			state[i] := GOLDEN; rsl[i] := 0;
		end;
	end;


// obtain a AUM pseudo-random value in [0..2**32]
function auRandom: Cardinal;
	begin
		auRandom := rsl[rcnt];
		inc(rcnt);
		if (rcnt=STSZ) then begin
			aum;
			rcnt := 0;
		end;
	end;


// seed AUM with a 512-bit block of 4-byte words (Bob Jenkins method) 
procedure auSeedW(seed: string; rounds: integer);
	var i,l: Cardinal;
		p: pointer;
	begin
		p:=@state[0];
		l:=Length(seed);
		if (l>STBYTES) then l:=STBYTES;
		auReset;
		for i:=0 to l-1 do
			byte((p+i)^) := byte(seed[i+1]);
		aum;
		for i:=1 to rounds do auRandom;  
	end;


// AUM # of bits internal state
function auStateBits: Cardinal;
	begin
		auStateBits := STBITS;
	end;


// AUM expected cycle length
function auCycle: Cardinal;
	begin
		auCycle := (STBITS+1) div 2;
	end;


// AUM Name
function auName: string;
	begin
		auName := NAME;
	end;

end.