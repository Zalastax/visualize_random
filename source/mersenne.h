#ifndef MERSENNE_H
#define MERSENNE_H
#include "uint.h"

uint32 randomMT(void);
uint32 reloadMT(void);
void seedMT(uint32 seed);
#endif