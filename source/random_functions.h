#ifndef RANDOM_FUNCTIONS_H
#define RANDOM_FUNCTIONS_H
#include "uint.h"
#include "mersenne.h"

typedef  uint32 (*random_function)(void);
typedef struct bob_rand_struct { uint32 a; uint32 b; uint32 c; uint32 d; } bob_rand_struct;

uint32 system_rand();
void s_lcrand(int x);
uint32 lcrand();
uint32 rand_lsfr();
uint32 rand_with_array();
uint32 xorshift();
void s_rand_qpr(unsigned int seedBase, unsigned int seedOffset);
uint32 rand_qpr();
uint32 conc_16_random();
uint32 rand_tausworth();
bob_rand_struct init_bob_rand();
uint32 autoranval();
#endif