// Grace Biemans & Nick Heleta
// geb965 & nwh397

#include <stdio.h>
#include "free.h"
#include "init.h"
freechunk* freeListHead;

/**
 * Function to free the given chunk
 * @param *p: the chunk to free
 * @return: 0 on success and -1 for failure
 */
int M_Free(void *p) {
    struct allocchunk *headPointer = (struct allocchunk *) p; // have a pointer to the chunk we are freeing
    struct freechunk *freePointer;

    if (headPointer->header.magicNumber != 1234567) {
        perror("Free failure, tried to free invalid chunk of memory\n");
        return -1;
    }
    else {
        // convert the alloc type chunk to free type chunk so we can put it in the freelist
        headPointer->header.magicNumber = 0;
        headPointer->footer.magicNumber = 0;
        freePointer = (struct freechunk *) headPointer;
        freePointer->footer.size = headPointer->footer.size;
        freePointer->header.next = NULL;
        freePointer->footer.previous = NULL;

        freechunk* start = freeListHead;
        int coalesced = 0;

        do {
            //printf("Start: %p\n", start);
            //printf("Start moved by size: %p\n", start + start->header.size);
            //printf("Start moved by size / 10: %p\n", start + ((start->header.size) / 10));
            //printf("Start moved by size + 32 / 10: %p\n", start + ((start->header.size + 32) / 10));
            //printf("Freepointer: %p\n", freePointer);
            // if there is a freelist chunk before it
            if (start + ((start->header.size + 32) / 10) == freePointer) {

                // if there is freelist chunk after, connect all 3 into 1
                // go through all the freelist chunks again and see if one is before the chunk we are freeing
                freechunk *start2 = freeListHead;
                do {

                    if (freePointer + ((freePointer->header.size + 32) / 10) == start2) {

                        start->header.size += freePointer->header.size + 32 + start2->header.size + 32;
                        start->footer.size += freePointer->header.size + 32 + start2->header.size + 32;

                        // take the behind node out of the freelist
                        if (start2 == freeListHead) {   // removing first node in freelist
                            freeListHead = start2->header.next;
                            start2->header.next = NULL;
                            freeListHead->footer.previous = NULL;
                        } else if (start2->header.next == NULL) {  // remove the last node in freelist
                            start2->footer.previous->header.next = NULL;
                            start2->footer.previous = NULL;
                        } else {  // remove a middle node from freelist
                            start2->footer.previous->header.next = start2->header.next->footer.previous;
                            start2->header.next->footer.previous = start2->footer.previous->header.next;
                            start2->footer.previous = NULL;
                            start2->header.next = NULL;
                        }
                        coalesced = 1;
                    }
                    else {
                        start2 = start2->header.next;
                    }
                } while (start2 != NULL);
                // if no chunk in freelist after it, connect 2 into 1
                if (!coalesced) {
                    start->header.size += freePointer->header.size + 32;
                    start->footer.size += freePointer->header.size + 32;
                    coalesced = 1;
                }
            }

            // else if there is a freelist chunk after the one we are freeing
            else if (freePointer + ((freePointer->header.size + 32) / 10) == start) {
                // check if there is a free chunk before it, connect 3 in 1
                freechunk *start2 = freeListHead;
                do {
                    if (start2 + ((start2->header.size) / 10) == freePointer) {
                        start2->header.size += freePointer->header.size + 32 + start->header.size + 32;
                        start2->footer.size += freePointer->header.size + 32 + start->header.size + 32;
                        // take the behind node out of the freelist
                        if (start == freeListHead) {   // removing first node in freelist
                            freeListHead = start->header.next;
                            start->header.next = NULL;
                            freeListHead->footer.previous = NULL;
                        } else if (start->header.next == NULL) {  // remove the last node in freelist
                            start->footer.previous->header.next = NULL;
                            start->footer.previous = NULL;
                        } else {  // remove a middle node from freelist
                            start->footer.previous->header.next = start->header.next->footer.previous;
                            start->header.next->footer.previous = start->footer.previous->header.next;
                            start->footer.previous = NULL;
                            start->header.next = NULL;
                        }
                        coalesced = 1;
                    } else {
                        start2 = start2->header.next;
                    }
                } while (start2 != NULL);

                freePointer->header.next = start->header.next;
                freePointer->footer.previous = start->footer.previous;
                freePointer->header.size += start->header.size + 32;
                freePointer->footer.size += start->header.size + 32;

                start->header.next = NULL;
                start->footer.previous = NULL;
                if (start == freeListHead) {
                    freeListHead = freePointer;
                }
                coalesced = 1;
            }
            // else there is no coalescing for that node in freelist, move to next node
            else {
                start = start->header.next;
            }
        } while (start != NULL && !coalesced);

        if (coalesced == 0) {   // add the chunk to the freelist as its own node
            freePointer->header.next = freeListHead;

            freeListHead->footer.previous = freePointer;
            freeListHead = freePointer;
        }
    }
    return 0;
}
