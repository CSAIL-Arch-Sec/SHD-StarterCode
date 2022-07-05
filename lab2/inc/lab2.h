#ifndef LAB2_H
#define LAB2_H

// Lab 2 for MIT 6.888 Spring 2022
// Joseph Ravichandran

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>

/************************
 * Lab 2 Helper Methods *
 ************************/

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
void clflush(void *addr);

/*
 * rdtsc
 * Reads the current timestamp counter
 *
 * Returns: Current value of TSC
 */
uint64_t rdtsc(void);

/*
 * time_access
 * Returns the time to access an address
 */
uint64_t time_access(void *addr);

/*
 * init_shared_memory
 * Intializes a region of shared memory by writing to it,
 * and then flushing it from the cache. This should
 * minimize any TLB related issues.
 *
 * Arguments:
 *  - shared_memory: Pointer to the region of memory to initialize
 *  - len: Length of shared memory region
 *
 * Returns: None
 * Side Effects: Brings shared_memory into the cache, and then flushes it.
 */
void init_shared_memory(char *shared_memory, size_t len);

/*
 * run_attacker
 *
 * Arguments:
 *  - kernel_fd: A file descriptor referring to the lab 2 vulnerable kernel module
 *  - shared_memory: A pointer to a region of memory shared with the server
 */
int run_attacker(int kernel_fd, char *shared_memory);

#endif // LAB2_H
