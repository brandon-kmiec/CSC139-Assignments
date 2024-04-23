#ifndef _UMEM_H
#define _UMEM_H

#include <stddef.h>

#define BEST_FIT 					(1)
#define WORST_FIT 					(2)
#define FIRST_FIT 					(3)
#define NEXT_FIT 					(4)
#define BUDDY						(5)

int 	umeminit(size_t sizeOfRegion, int allocationAlgo);
void 	*umalloc(size_t size);
int 	ufree(void *ptr);
void 	umemdump();

#endif
