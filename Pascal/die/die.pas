// die: generate a bit-stream file of pseudo-random values for DIEHARD tests.
// Usage:   >die <rng #> <seed> <# of bitsreams>
// Example: >die 4 "my seed" 100
//(seed BB512 with seed "my seed" and generate a file named BB512.32)
// rng # in [0..14] 0=CONG, 2=ISAAC, 3=BB128, 4=BB256, 5=BB512
// Output: a file named <rng-name>.32 (Example: KISS.32)
program die;

uses rng, SysUtils;

const 	blen= 16777215;//33554431; // 16777215; // desired file-length (bytes*4)

var 	key: string = 'FSBiaccenRI';
		ng : TRNG = ALEF;
		words: array[0..blen] of Cardinal;
		P: pointer;
		i,nw,total,fSz: Cardinal;
		fOut: file;
		fname: string = 'die.32';
		// # of bitstreams for NIST tests
		bss: Cardinal = 100;
		
procedure Usage;
	begin
		Writeln('die: generate a bit-stream file of pseudo-random values for DIEHARD tests.');
		Writeln('Usage  : >die <rng #> <seed> <# of bitsreams>');
		Writeln('Example: >die 4 "my seed" 100');
		Writeln('(seed BB512 with seed "my seed" and generate a file named BB512.32)');
		Writeln('(rng # in [0..14] 0=CONG, 2=ISAAC, 3=BB128, 4=BB256, 5=BB512)');
		Writeln('Output: a file named <rng-name>.32 (Example: KISS.32)');
	end;
	
begin
	// check the command line
	if ParamCount <1 then begin Usage; Halt; end;
	if ParamCount>=1 then if ParamStr(1)[1] in ['a'..'z','A'..'Z','-'] then begin Usage; Halt; end;
	if ParamCount>=1 then ng := TRNG(StrToInt(ParamStr(1)));
	if ParamCount>=2 then key := ParamStr(2);
	if ParamCount>=3 then bss := StrToInt(ParamStr(3));
	// set the output filename
	fname := rngs[ng] + '.32';
	// seed all available RNGs
	rSeedAll(key);
	// generate a pseudo-random bitstream from the selected PRNG
	for i := 0 to blen do
		words[i] := rRandom(ng);
	// prepare and write the bitstream to file
	P := @words[0];
	Assign(fOut,fname); Rewrite(fOut,1);
	fSz := SizeOf(words);
	total:=0;
	repeat
		BlockWrite(fOut,P^,fSz,nw);
		INC(total,nw);
	until (nw=0) or (total>=fSz);
	Close(fOut);
	Writeln(IntToStr(total)+' bytes written to <'+fname+'>. ');
	// NIST Section output
	// NIST divides a binary sequence into a user-specified number of bitsreams.
	// So here we help the user by outputting their sizes based on the total bytes written.
	Writeln(bss,' bitstreams of length ',total div bss);
end.
