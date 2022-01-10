// Grace Biemans & Nick Heleta
// geb965 & nwh397


#include <stdio.h>
#include <sys/mman.h>
#include "init.h"
#include "alloc.h"
freechunk* freeListHead;
allocchunk* freeChunk;
freechunk* lastChecked;

/**
 * Function rounds up to nearest multiple of 16
 * @param num: The number we are rounding up
 * @return: The multiple of 16
 */
int roundUp16(int num) {
    int remainder = num % 16;
    if (remainder == 0) {
        return num;
    }
    return num + 16 - remainder;
}

/**
 * Function to initialize the memory allocator
 * @param size: size of the memory space in bytes that the allocator should manage
 * @return: 0 on success and -1 for a failure
 */
int M_Init(int size) {
    size = roundUp16(size);


    if (freeListHead != NULL) {
        perror("Initialization error: freelist has already been initialized");
        return -1;
    }

    freeListHead = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, 0, 0);
    if (freeListHead == MAP_FAILED) {
        perror("Mapping failed\n");
        return -1;
    }
    else {  // have the whole space held as a chunk with header and footer (this is the freelist)
        // first, initialize the list, should have one entry of the size of all the space minus header and footer size for one
        freeListHead->header.next = NULL;
        freeListHead->header.size = size - 32;

        freeListHead->footer.previous = NULL;
        freeListHead->footer.size = size - 32;
    }

    lastChecked = freeListHead;

    return 0;
}
