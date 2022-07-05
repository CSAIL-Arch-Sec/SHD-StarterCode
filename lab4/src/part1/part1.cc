#include "part1.hh"

/*
 *
 * DO NOT MODIFY
 *
 */

#define NUM_ITERATIONS 100

int main(int argc, char** argv) {
  setvbuf(stdout, NULL, _IONBF, 0);

  if (getuid()) {
    printf("ERROR: Root permissions required!\n");
    exit(1);
  }

  // Allocate a large pool of memory (of size BUFFER_SIZE_MB) pointed to by allocated_mem
  allocated_mem = allocate_pages(BUFFER_SIZE_MB * 1024UL * 1024UL);
 
  // Setup, then verify the PPN_VPN_map
  setup_PPN_VPN_map(allocated_mem, PPN_VPN_map);
  verify_PPN_VPN_map(allocated_mem, PPN_VPN_map);

  // Now, run all of the experiments!
  int sum_AB = 0;
  for(int i = 0; i < NUM_ITERATIONS; i++) {
    sum_AB += hammer_addresses(VICT_ADDR, ADDR_A, ADDR_B);
  }
  printf("Number of bit-flip successes observed out of %d (A/B): %d\n", NUM_ITERATIONS, sum_AB); 
  
  int sum_AC = 0;
  for(int i = 0; i < NUM_ITERATIONS; i++) {
    sum_AC += hammer_addresses(VICT_ADDR, ADDR_A, ADDR_C);
  }
  printf("Number of bit-flip successes observed out of %d (A/C): %d\n", NUM_ITERATIONS, sum_AC); 
  
  int sum_AD = 0;
  for(int i = 0; i < NUM_ITERATIONS; i++) {
    sum_AD += hammer_addresses(VICT_ADDR, ADDR_A, ADDR_D);
  }
  printf("Number of bit-flip successes observed out of %d (A/D): %d\n", NUM_ITERATIONS, sum_AD); 
}
