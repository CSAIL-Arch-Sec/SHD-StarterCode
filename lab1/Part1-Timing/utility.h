#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>

#ifndef __UTILITY_H__
#define __UTILITY_H__

#define SAMPLES 10


// Function to read the time stamp counter, which is called tsc for short
// "rdtscpp" returns a 32bit unsigned integer
// "rdtscpp64" return a 64 bit unsigned integer
// Details in https://www.felixcloutier.com/x86/rdtscpp
static inline uint32_t rdtscpp() {
    uint32_t rv;
    asm volatile ("rdtscpp": "=a" (rv) :: "edx", "ecx");
    return rv;
}

static inline uint64_t rdtscpp64() {
    uint32_t low, high;
    asm volatile ("rdtscpp": "=a" (low), "=d" (high) :: "ecx");
    return (((uint64_t)high) << 32) | low;
}

// Function "lfence" wrap the assembly instruction lfence
// This function performs a serializing operation which ensures that
// the instructions after "lfence" start execution after
// all the instructions before "lfence" complete
// Details in https://www.felixcloutier.com/x86/lfence
static inline void lfence() {
    asm volatile("lfence");
}

// Here is an example of using "rdtscp" and "mfence" to
// measure the time it takes to access a block specified by its virtual address
// The corresponding pseudo code is
// =========
// t1 = rdtscp
// load addr
// t2 = rdtscp
// cycles = t2 - t1
// return cycles
// =========
static inline uint64_t measure_one_block_access_time(uint64_t addr)
{
    uint64_t cycles;

    asm volatile("mov %1, %%r8\n\t"
    "mfence\n\t"
    "lfence\n\t"
    "rdtscp\n\t"
    "mov %%eax, %%edi\n\t"
    "mov (%%r8), %%r8\n\t"
    "rdtscp\n\t"
    "sub %%edi, %%eax\n\t"
    : "=a"(cycles) /*output*/
    : "r"(addr)    /*input*/
    : "r8", "edi"); /*reserved register*/

    return cycles;
}

static inline uint64_t one_block_access(uint64_t addr)
{
    asm volatile("mov (%0), %%r8\n\t"
    : /*output*/
    : "r"(addr)    /*input*/
    : "r8");    /*reserved register*/

}


// A wrapper function of the clflush instruction
// The instruction evict the given address from the cache to DRAM
// so that the next time the line is accessed, it will be fetched from DRAM
// Details in https://www.felixcloutier.com/x86/clflush
static inline void clflush(void *v) {
    asm volatile ("clflush 0(%0)": : "r" (v):);
}




// Supporting functions for printing results in different formats
// Function "compare" is used in the priting functions and you do not need it
int compare(const void *p1, const void *p2) {
    uint64_t u1 = *(uint64_t *)p1;
    uint64_t u2 = *(uint64_t *)p2;

    return (int)u1 - (int)u2;
}


// Print out the latencies you measured
void print_results(uint64_t* dram, uint64_t* l1, uint64_t* l2, uint64_t* l3) {
    qsort(dram, SAMPLES, sizeof(uint64_t), compare);
    qsort(l1, SAMPLES, sizeof(uint64_t), compare);
    qsort(l2, SAMPLES, sizeof(uint64_t), compare);
    qsort(l3, SAMPLES, sizeof(uint64_t), compare);
    printf("             :  L1   L2   L3   Mem   \n");
    printf("Minimum      : %5ld %5ld %5ld %5ld\n", l1[0], l2[0], l3[0], dram[0]);

    printf("Bottom decile: %5ld %5ld %5ld %5ld\n", l1[SAMPLES/10], l2[SAMPLES/10],
                                                l3[SAMPLES/10], dram[SAMPLES/10]);

    printf("Median       : %5ld %5ld %5ld %5ld\n", l1[SAMPLES/2], l2[SAMPLES/2],
                                                l3[SAMPLES/2], dram[SAMPLES/2]);

    printf("Top decile   : %5ld %5ld %5ld %5ld\n", l1[(SAMPLES * 9)/10], l2[(SAMPLES * 9)/10],
                                                   l3[(SAMPLES * 9)/10], dram[(SAMPLES * 9)/10]);

    printf("Maximum      : %5ld %5ld %5ld %5ld\n", l1[SAMPLES-1], l2[SAMPLES-1],
                                                    l3[SAMPLES-1], dram[SAMPLES-1]);
}

// Format the latencies for part 1.5
void print_results_for_python(uint64_t* dram, uint64_t* l1, uint64_t* l2, uint64_t* l3)
{
    qsort(dram, SAMPLES, sizeof(uint64_t), compare);
    qsort(l1,   SAMPLES, sizeof(uint64_t), compare);
    qsort(l2,   SAMPLES, sizeof(uint64_t), compare);
    qsort(l3,   SAMPLES, sizeof(uint64_t), compare);

    for (int i = 0; i < SAMPLES; i++) {
        printf("%ld ", l1[i]);
    }
    printf("\n");
    for (int i = 0; i < SAMPLES; i++) {
        printf("%ld ", l2[i]);
    }
    printf("\n");
    for (int i = 0; i < SAMPLES; i++) {
        printf("%ld ", l3[i]);
    }
    printf("\n");
    for (int i = 0; i < SAMPLES; i++) {
        printf("%ld ", dram[i]);
    }
    printf("\n");
}

#endif // _UTILITY_H__ 
