// Grace Biemans & Nick Heleta
// geb965 & nwh397


#include <stdio.h>
#include "display.h"
#include "init.h"
freechunk* freeListHead;

/**
 * Function that prints the addresses and sizes of the free chunks to stdout
 */
void M_Display() {
    // will print out the chunks in the freelist

    if (freeListHead == NULL) {
        printf("freelist is empty\n");
    }

    else {
        printf("Nodes in the freelist:\n");
        freechunk* start = freeListHead;

        // only one chunk in freelist
        if (start->header.next == NULL) {
            printf("  Node 1:\n    Address: %p\n    Size: %d\n", start, start->header.size);
        }

        else {
            int nodeNum = 1;
            while (start->header.next != NULL) {
                printf("  Node %d:\n    Address: %p\n    Size: %d\n", nodeNum, start, start->header.size);
                start = start->header.next;
                nodeNum ++;
            }
            printf("  Node %d:\n    Address: %p\n    Size: %d\n", nodeNum, start, start->header.size);
        }
    }
    printf("\n");
}
