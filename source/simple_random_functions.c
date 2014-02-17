#include "random_functions.h"
#include <stdlib.h> 
uint32 system_rand(){
	return rand();
}
	
	int rseed = 0;
 
void s_lcrand(int x)
{
	rseed = x;
}
 
 
uint32 lcrand()
{
	return rseed = (rseed * 1103515245 + 12345) & RAND_MAX;
}

uint32 rand_lsfr()
{
	static unsigned bit;
	static uint32 lfsr = 0xACE1u;
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr =  (lfsr >> 1) | (bit << 15);
}
#pragma GCC diagnostic ignored "-Wuninitialized"
uint32 rand_with_array(){
	static uint32 r_num[50000];
	static int index;
	static uint32 return_value;
	index %=50000;
	uint32 index_for_sub = (index-1103483)*2103449;
	index_for_sub%=50000;
	uint32 index_for_add =  index*return_value-r_num[index_for_sub];
	index_for_add %=50000;
	return_value+= r_num[index]+r_num[index_for_add];
	r_num[index] = return_value+1103515211;
	return return_value;
}
uint32 xorshift(void) {
  static uint32 x = 123456789;
  static uint32 y = 362436069;
  static uint32 z = 521288629;
  static uint32 w = 88675123;
  uint32 t;

  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}
 
 
 
    static uint32 m_index;
    static uint32 m_intermediateOffset;

    uint32 permuteQPR(unsigned int x)
    {
        static const unsigned int prime = 4294967291u;
        if (x >= prime)
            return x; // The 5 integers out of range are mapped to themselves.
        unsigned int residue = ((unsigned long long) x * x) % prime;
        return (x <= prime / 2) ? residue : prime - residue;
    }

    void s_rand_qpr(unsigned int seedBase, unsigned int seedOffset)
    {
        m_index = permuteQPR(permuteQPR(seedBase) + 0x682f0161);
        m_intermediateOffset = permuteQPR(permuteQPR(seedOffset) + 0x46790905);
    }

    uint32 rand_qpr()
    {
        return permuteQPR((permuteQPR(m_index++) + m_intermediateOffset) ^ 0x5bf03635);
    }
	
	
	/* concatenation of following two 16-bit multiply with carry generators */
/* x(n)=a*x(n-1)+carry mod 2^16 and y(n)=b*y(n-1)+carry mod 2^16, */
/* number and carry packed within the same 32 bit integer.        */
/******************************************************************/

static uint32 SEED_X = 521288629;
static uint32 SEED_Y = 362436069;


uint32 conc_16_random ()
{
/* Use any pair of non-equal numbers from this list for "a" and "b"
    18000 18030 18273 18513 18879 19074 19098 19164 19215 19584       
    19599 19950 20088 20508 20544 20664 20814 20970 21153 21243       
    21423 21723 21954 22125 22188 22293 22860 22938 22965 22974       
    23109 23124 23163 23208 23508 23520 23553 23658 23865 24114       
    24219 24660 24699 24864 24948 25023 25308 25443 26004 26088       
    26154 26550 26679 26838 27183 27258 27753 27795 27810 27834       
    27960 28320 28380 28689 28710 28794 28854 28959 28980 29013       
    29379 29889 30135 30345 30459 30714 30903 30963 31059 31083
*/
   static unsigned int a = 18000, b = 30903;

   SEED_X = a*(SEED_X&65535) + (SEED_X>>16);
   SEED_Y = b*(SEED_Y&65535) + (SEED_Y>>16);

   return ((SEED_X<<16) + (SEED_Y&65535));
}


void rand_seed_conc_16 (uint32 seed1, uint32 seed2)
{
  if (seed1) SEED_X = seed1;   /* use default seeds if parameter is 0 */
  if (seed2) SEED_Y = seed2;
}

/* Combination of 3 tausworth generators -- assumes 32-bit integers */

static uint32 s1=390451501, s2=613566701, s3=858993401;  /* The seeds */
static uint32 mask1, mask2, mask3;
static uint32 shft1, shft2, shft3, k1=31, k2=29, k3=28;

/* use either of the following two sets of parameters*/
static uint32 q1=13, q2=2, q3=3, p1=12, p2=4, p3=17;
/* static int q1=3, q2=2, q3=13, p1=20, p2=16, p3=7; */


uint32 rand_tausworth()
{
   uint32 b;
  
   b  = ((s1 << q1)^s1) >> shft1;
   s1 = ((s1 & mask1) << p1) ^ b;
   b  = ((s2 << q2) ^ s2) >> shft2;
   s2 = ((s2 & mask2) << p2) ^ b;
   b  = ((s3 << q3) ^ s3) >> shft3;
   s3 = ((s3 & mask3) << p3) ^ b;
   return (s1 ^ s2 ^ s3);
}


void rand_seed_tausworth (uint32 a, uint32 b, uint32 c)
{
    static uint32 x = 4294967295U;

    shft1=k1-p1;
    shft2=k2-p2;
    shft3=k3-p3;
    mask1 = x << (32-k1);
    mask2 = x << (32-k2);
    mask3 = x << (32-k3);
    if (a > (uint32)(1<<shft1)) s1 = a;
    if (b > (uint32)(1<<shft2)) s2 = b;
    if (c > (uint32)(1<<shft3)) s3 = c;
    rand();
}


