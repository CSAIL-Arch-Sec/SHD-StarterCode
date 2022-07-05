#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include <emmintrin.h>
#include <x86intrin.h>

#include "lab2.h"
#include "lab2ipc.h"

/*
 * mfence
 * Adds a memory fence
 */
static inline void mfence() {
    asm volatile("mfence");
}

/*
 * clflush
 * Flushes an address from the cache for you
 *
 * Arguments:
 *  - addr: A virtual address whose cache line we should flush
 *
 * Returns: None
 * Side Effects: Flushes a cache line from the cache
 */
void clflush(void *addr) {
    _mm_clflush(addr);
}

/*
 * rdtsc
 * Reads the current timestamp counter
 *
 * Returns: Current value of TSC
 */
uint64_t rdtsc(void) {
    return __rdtsc();
}

/*
 * time_access
 * Returns the time to access an address
 */
uint64_t time_access(void *addr) {
    unsigned int tmp;
    uint64_t time1, time2;
    time1 = __rdtscp(&tmp);
    tmp = *(unsigned int *)addr;
    time2 = __rdtscp(&tmp);
    return time2 - time1;
}

/*
 * init_shared_memory
 * Load all the pages of shared memory, initialize them, and flush them from the cache.
 *
 * Arguments:
 *  - shared_memory: Pointer to a shared memory region
 *  - len: Size of the shared memory region to initialize
 *
 * Returns: None
 * Side Effects: Initializes some memory and flushes it from the cache.
 */
void init_shared_memory(char *shared_memory, size_t len) {
    for (int i = 0; i < (len / LAB2_PAGE_SIZE); i++) {
        shared_memory[LAB2_PAGE_SIZE * i] = 0x41;
        clflush(&shared_memory[LAB2_PAGE_SIZE * i]);
    }
}
