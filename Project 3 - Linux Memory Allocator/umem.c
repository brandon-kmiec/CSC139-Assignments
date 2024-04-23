// Brandon Kmiec
// CSC 139-06
// 04-15-2024
// Linux Memory Allocator

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

#include "umem.h"

typedef struct memBlock {
    size_t size;
    struct memBlock *next;
}memBlock;

static void *memMap = NULL;
static memBlock *freeList = NULL;
static memBlock *firstFitEndLoc = NULL;
static int allocAlgo;

size_t roundNum(int, size_t);

// initialize memory region with allocationAlgo
int umeminit(size_t sizeOfRegion, int allocationAlgo) {
    // failure if sizeOfRegion <= 0 or if umeminit called more than once 
    // (determined if memMap != NULL)
    if(sizeOfRegion <= 0 || memMap != NULL) {
        return -1;
    } // end if

    // failure if invalid allocationAlgo
    if(allocationAlgo < 1 || allocationAlgo > 4) {
        return -1;
    } // end if

    // assign value for allocAlgo
    allocAlgo = allocationAlgo;

    // round sizeOfRegion up to be in units of the page size
    sizeOfRegion = roundNum(getpagesize(), sizeOfRegion);

    // request sizeOfRegion bytes of memory from OS
    int fd = open("/dev/zero", O_RDWR);
    memMap = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if(memMap == MAP_FAILED) {
        return -1;
    } // end if
    close(fd);

    // assign values for freeList
    freeList = (memBlock *)memMap;
    //freeList -> size = sizeOfRegion;
    freeList -> size = sizeOfRegion - sizeof(memBlock);
    freeList -> next = NULL;

    // success 
    return 0;
} // end umeminit


// allocate memory based on free space in freeList
void *umalloc(size_t size) {
    // failure if invalid size
    if(size <= 0) {
        return NULL;
    } // end if

    // failure if not enough free space allocated by umeminit
    if(size > freeList -> size) {
        return NULL;
    } // end if

    // round size up to next multiple of 8
	size = roundNum(8, size);
	
    memBlock *curMemBlock = freeList;
    memBlock *retMemBlock = NULL;

    size_t bestSize;

    switch(allocAlgo) {
		// search freeList for the first smallest free space chunk that is large
		// enough for the requested size
        case BEST_FIT:
            bestSize = freeList -> size + size;
            while(curMemBlock != NULL) {
                if(curMemBlock -> size < bestSize && curMemBlock -> size >= size) {
                    retMemBlock = curMemBlock;
                    bestSize = curMemBlock -> size;
                } // end if
                curMemBlock = curMemBlock -> next;
            } // end while
            break;
		// search freeList for the largest chunk of free space
        case WORST_FIT:
            bestSize = 0;
            while(curMemBlock != NULL) {
                if(curMemBlock -> size > bestSize && curMemBlock -> size >= size) {
                    retMemBlock = curMemBlock;
                    bestSize = curMemBlock -> size;
                } // end if
                curMemBlock = curMemBlock -> next;
            } // end while
            break;
		// search freeList for the first chunk that is large enough and split
        case FIRST_FIT:
            while(curMemBlock != NULL) {
                if(curMemBlock -> size >= size) {
                    firstFitEndLoc = curMemBlock;
                    retMemBlock = curMemBlock;
					
					curMemBlock -> next = freeList;
					curMemBlock -> next -> size = curMemBlock -> size - size;
					curMemBlock -> size = size;
                    break;
                } // end if
                curMemBlock = curMemBlock -> next;
            } // end while
			freeList = curMemBlock;
            break;
		// search freeList for the second chunk that is large enough and split
        case NEXT_FIT:
			while(curMemBlock != NULL) {
                if(curMemBlock -> size >= size) {
                    firstFitEndLoc = curMemBlock;
                    retMemBlock = curMemBlock;
					
					curMemBlock -> next = freeList;
					curMemBlock -> next -> size = curMemBlock -> size - size;
					curMemBlock -> size = size;
                    break;
                } // end if
                curMemBlock = curMemBlock -> next;
            } // end while
			freeList = curMemBlock;
		
            curMemBlock = retMemBlock;
            while(curMemBlock != NULL) {
                if(curMemBlock -> size >= size) {
                    retMemBlock = curMemBlock;
					
					curMemBlock -> next = freeList;
					curMemBlock -> next -> size = curMemBlock -> size - size;
					curMemBlock -> size = size;
                    break;
                } // end if
                curMemBlock = curMemBlock -> next;
            } // end while
			freeList = curMemBlock;
            break;
    } // end switch

    return (void *)retMemBlock;
} // end *umalloc


// free previously allocated memory
int ufree(void *ptr) {
    // invalid ptr
    if(ptr == NULL) {
        return -1;
    } // end if

	// Coalesce if allocAlgo is FIRST_FIT or NEXT_FIT
	if(allocAlgo == FIRST_FIT || allocAlgo == NEXT_FIT) {
		memBlock *curMemBlock = (memBlock *) ptr;
		memBlock *prevMemBlock = freeList;
		memBlock *nextMemBlock = curMemBlock -> next;

		while(prevMemBlock -> next != curMemBlock) {
			prevMemBlock = prevMemBlock -> next;
		} // end while
		curMemBlock -> size += prevMemBlock -> size + nextMemBlock -> size;
		prevMemBlock -> next = nextMemBlock -> next;
		curMemBlock -> next = nextMemBlock -> next;
	} // end if
	
    memBlock *curMemBlock = (memBlock *) ptr;
    curMemBlock = freeList;
    freeList = curMemBlock;

    return 0; // temp return
} // end ufree


// print regions of free memory
void umemdump() {
    memBlock *temp = freeList;

    while(freeList != NULL) {
        printf("%p: %zu\n", freeList, freeList -> size);
        freeList = freeList -> next;
    } // end while

    freeList = temp;
} // end umemdump


// helper function to round up numbers
size_t roundNum(int divisor, size_t dividend) {
    size_t ret;
	
	int remainder = dividend % divisor;
    int quotient = dividend / divisor;
    if(remainder == 0) {
        ret = quotient * divisor;
    } // end if
    else {
        ret = (quotient + 1) * divisor;
    } // end else
		
	return ret;
} // end roundNum
