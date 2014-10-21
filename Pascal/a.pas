// m: Check output from the ALEPH CSPRNG 
program a;
uses aleph;
{ $define STREAM}
// mixing rounds: 7 is recommended minimum
const ROUNDS = 7;
var n: Cardinal;
	seed: string = 'Bacon';
begin
	// pull a seed from the command line
	if ParamCount>0 then seed:=ParamStr(1);
	// print some information
	Writeln('CSPRNG        : ',alName);
	Writeln('Internal state: ',alStateBits,' bits');
	Writeln('Minimum cycle : 2**',alCycle);
	Writeln('Maximum cycle : 2**',alStateBits);
	Writeln('Average cycle : 2**',alStateBits-2);
	// seed MOTE and mix
	alSeedW(seed,8*ROUNDS);
	{$ifdef STREAM}
	// show some MOD 26 stream output
	for n:=1 to 672 do
		Write(chr(alRandom mod 26 + ord('A')));
	Writeln;
	{$endif}
end.
