Results of George Marsaglia's canonical DIEHARD suite of tests on CSPRNGs ISAAC,  ALEPH16, ALEPH32 and ALEPH64, plus Marsaglia's own PRNG KISS. ISAAC is used here as the "gold standard" of comparison and KISS as another sanity check. CONG (a linear congruential generator) is included as an example of a wanting PRNG which fails at least half of the DIEHARD battery.

The size of the bitstream given to DIEHARD was 2**24 32-bit words in a file of 64 MB. Doubling the bitstream length had no effect on the test results, suggesting that the program truncates it.

Various keys were used to seed the PRNGs and the test results for each placed in separate folders. Changing the seed changed the p-values significantly in every case.

1)64MB - seed = "something very silly"
2)64MB - seed = "FSBiaccenRI"
3)64MB - seed = "BOte-swaine."
4)64MB - seed = "Heere Master: What cheere?"

The program used to generate the bitstream files was die.pas, which can be found in the ALEPH distribution under the Pascal directory.

C.C.Kayne
cckayne@gmail.com
