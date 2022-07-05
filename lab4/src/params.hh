#ifndef PARAMS_GUARD
#define PARAMS_GUARD

// Size of allocated buffer
#define BUFFER_SIZE_MB 2048

// Size of hugepages in system
#define HUGE_PAGE_SIZE (1 << 21)

// Size of DRAM row (1 bank)
#define ROW_SIZE (8192)

// Number of hammers to perform per iteration
#define HAMMERS_PER_ITER 5000000

#endif
