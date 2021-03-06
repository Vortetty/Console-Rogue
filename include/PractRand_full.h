/*
I, Chris Doty-Humphrey, release all of my work on PractRand to the 
public domain.  

All statistical tests in PractRand are wholly my own work, and are 
public domain.  

All the auxillary code, including the command line tools, are my own 
work and public domain.  

All of the recommended RNGs are unencumbered.  I do not know of any 
patents covering any part of them:
	jsf* was designed by Robert Jenkins, and he has released it to 
	the public domain.  The implementation is mostly his code with a 
	little of my own.  

	isaac* was designed by Robert Jenkins, and he has released it to 
	the public domain.  The implementation is mostly his code with a 
	little of my own.  

	sfc* was designed and coded by myself, and is public domain.  

	efiix* was designed and coded by myself, and is public domain.  

	hc256: From the eSTREAM project page: "The stream ciphers HC-128 and 
	HC-256 are not patented and are royalty free. Anyone can use HC-128 
	and HC-256 free of charge."
	I started with the reference implementation and modified it slightly.  

	mt19937 is an unencumbered algorithm.  The implementation included 
	is my own, and is public domain.  

	trivium: From the eSTREAM project page: "To the author's knowledge 
	Trivium is not covered by any patent. Trivium is also free available 
	for any use."
	The implementation included is my own.  

	xsm* was written by myself, incorporating some ideas from svfuerst on 
	usenet comp.lang.asm.x86, and is public domain.  

	salsa: From the eSTREAM project page: "My policy is that Salsa20 is 
	free for everyone to use. I am not aware of any patents or patent 
	applications covering Salsa20. I will update the ECRYPT Stream Cipher 
	project when necessary."
	see also: http://cr.yp.to/snuffle/ip.pdf
	The implementation included is my own.  
	
	chacha: I believe the author intended this to be covered under the 
	same license as salsa, but unfortunately I can't find any explicit 
	statement to that effect.  I think he hasn't gotten around to it 
	yet.  If the IP situation is important to you, use Salsa instead 
	just in case.  They have identical interfaces and features so that 
	should be no issue.  
	The implementation included is my own.  

The "NotRecommended" / "other" RNGs included in the full version... are 
more numerous and less important - they are intended for comparison 
purposes, and not intended to actually ship with any real product.  The 
code for them was either written by myself or copied from a publicly 
accessible web page, forum or usenet post, or academic paper.  I do 
not know of any patent situation involving the algorithms, but have not 
paid any great attention to them.  



(license added by Winter from http://pracrand.sourceforge.net/license.txt)
*/

#ifndef _PractRand_full_h
#define _PractRand_full_h

#include "PractRand/config.h"
#include "PractRand/rng_basics.h"
#include "PractRand/rng_helpers.h"
#include "PractRand/test_helpers.h"
#include "PractRand/test_batteries.h"
//#include "PractRand/rng_adaptors.h"
#include "PractRand/tests.h"
#include "PractRand/test_batteries.h"

#endif //_PractRand_full_h
