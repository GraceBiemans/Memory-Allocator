// Grace Biemans & Nick Heleta
// geb965 & nwh397

typedef struct freechunk{
    struct freeheader_ {
        int size;           // 4 bytes
        struct freechunk* next;   // 8 bytes
    }header;

    struct freefooter_ {
        int size;
        struct freechunk* previous;
    }footer;
} freechunk;

typedef struct allocchunk{
    struct header_ {
        int size;           // 4 bytes
        int magicNumber;    // 4 bytes
    }header;

    struct footer_ {
        int size;
        int magicNumber;
    }footer;
} allocchunk;

/**
 * Function to initialize the memory allocator
 * @param size: size of the memory space in bytes that the allocator should manage
 * @return: 0 on success and -1 for a failure
 */
int M_Init(int size);

/**
 * Function rounds up to nearest multiple of 16
 * @param num: The number we are rounding up
 * @return: The multiple of 16
 */
int roundUp16(int num);

extern freechunk* freeListHead;
extern allocchunk* freeChunk;
extern freechunk* lastChecked;

