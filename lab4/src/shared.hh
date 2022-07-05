#ifndef SHARED_GUARD
#define SHARED_GUARD

#include <fcntl.h>
#include <inttypes.h>
#include <map>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include <unistd.h>
#include <vector>
#include <cstdint>
#include <emmintrin.h>

// Physical Page Number to Virtual Page Number Map
extern std::map<uint64_t, uint64_t> PPN_VPN_map;

// Base pointer to a large memory pool
extern void * allocated_mem;

// Student Provided Functions
uint64_t virt_to_phys(uint64_t virt_addr);
uint64_t phys_to_virt(uint64_t phys_addr);
uint8_t phys_to_bankid(uint64_t phys_ptr, uint8_t candidate);
void setup_PPN_VPN_map(void * mem_map, std::map<uint64_t, uint64_t> &PPN_VPN_map);
uint64_t measure_bank_latency(uint64_t addr_A, uint64_t addr_B);

// Helper Functions
void * allocate_pages(uint64_t memory_size);


// Staff Provided Helper Functions
inline uint64_t get_time() {
	uint64_t cycles;
	asm volatile("rdtscp\n\t"
				 "shl $32, %%rdx\n\t"
				 "or %%rdx, %0\n\t"
				 : "=a"(cycles)
				 :
				 : "rcx", "rdx", "memory");

	return cycles;
}

#endif
