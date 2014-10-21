Bob Jenkins' countx is without a doubt the most gruelling, revealing and effective test any pseudo-random function can be made to undergo. For this reason, it is the first test to apply when developing a PRNG. Experience has shown that if a generator can pass countx for sub-sequences 2-12 in 2**32 output values and above, it is practically guaranteed to emerge unscathed from any other test battery one might decide to subject it to, including NIST and DIEHARD (the industry-standards).

Jenkins, who adapted countx from a test of Bob Blorp2's, describes it thus: "As near as I can tell, the bitcounting test is detecting failure to achieve sufficient overall avalanche. It is noticing that bitcounts between nearby values don't change unpredictably enough. Unlike NDA or the gap or run test, it's nearly immune to what shift constants I choose. It doesn't care where the bits get set, so long as they get set somewhere ... The same thing is in Marsaglia's DIEHARD, Count-The-1s, except it was counting bits in bytes, for 5 consecutive bytes. Also Marsaglia tries to account for overlapping subsequence being correlated. My implementation ignores that, so mine has an acceptable range of about -5..5 instead of -3..3. Marsaglia's version could catch problems with a state of one 32-bit value, but not a state with four 32-bit values."

/* countx.c
 * By Bob Jenkins, public domain, implementing Bob Blorp2's public domain test.
	
 * Given a subsequence of n 32-bit random numbers, compute the number
 * of bits set in each term, reduce that to low, medium or high number
 * of bits, and concatenate a bunch of those 3-item buckets.
 * Do this for len overlapping n-value sequences.  And report the chi-square
 * measure of the results compared to the ideal distribution.
 
 * Extended with multiple-round tabulation by C.C.Kayne, September 2014.
	Usage: 	>countx <# rounds> <min terms> {<max terms>} {<# trials>} 
	Example:>countx 32 2 12 3
	(Perform 3 trials, from 2**32 to 2**34 sequences, from 2 to 12 terms per sequence.)
*/
