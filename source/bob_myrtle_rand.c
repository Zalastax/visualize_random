#include "random_functions.h"
#include <time.h>

#define rot(x,k) (((x)<<(k))|((x)>>(32-(k))))
uint32 ranval( bob_rand_struct *x ) {
    uint32 e = x->a - rot(x->b, 27);
    x->a = x->b ^ rot(x->c, 17);
    x->b = x->c + x->d;
    x->c = x->d + e;
    x->d = e + x->a;
    return x->d;
}


void raninit( bob_rand_struct *x, uint32 seed ) {
    uint32 i;
    x->a = 0xf1ea5eed, x->b = x->c = x->d = seed;
    for (i=0; i<20; ++i) {
        (void)ranval(x);
    }
}

bob_rand_struct r;
bob_rand_struct init_bob_rand(){
	raninit(&r, time(NULL));
	return r;
}
uint32 autoranval(){
	return ranval(&r);
}