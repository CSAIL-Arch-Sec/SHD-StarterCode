#include "../shared.hh"
#include "../params.hh"

#define SAMPLES 10

/*
 *
 * DO NOT MODIFY
 *
 */

void print_results(uint64_t* same, uint64_t* diff);
void print_to_json(uint64_t* same, uint64_t* diff);

int main (int ac, char **av) {
  
  setvbuf(stdout, NULL, _IONBF, 0);

  if (getuid()) {
    printf("ERROR: Root permissions required!\n");
    exit(1);
  }

  // Allocate a large pool of memory (of size BUFFER_SIZE_MB) pointed to by allocated_mem
  allocated_mem = allocate_pages(BUFFER_SIZE_MB * 1024UL * 1024UL);
 
  // Setup PPN_VPN_map
  setup_PPN_VPN_map(allocated_mem, PPN_VPN_map);

  uint64_t same_bank_latency[SAMPLES] = {0};
  uint64_t diff_bank_latency[SAMPLES] = {0};

  // Provided Physical Addresses
  // You don't need to change these!
  uint64_t addr_A = 0x7535a000UL;
  uint64_t addr_B = 0x75396000UL; // Same bank, different row ID as A
  uint64_t addr_C = 0x75358000UL; // Diff bank, same row ID as A
   
  // Determine Mapped Virtual Addresses
  uint64_t addr_A_ptr = phys_to_virt(addr_A);
  uint64_t addr_B_ptr = phys_to_virt(addr_B);
  uint64_t addr_C_ptr = phys_to_virt(addr_C);

  //Calculate bank latencies
  // First, measure bank latencies for addresses in the same bank 
  for (int i = 0; i < SAMPLES; i++) 
    same_bank_latency[i] = measure_bank_latency(addr_A_ptr, addr_B_ptr);
 
  // Second, measure bank latencies for addresses in different banks
  for (int i = 0; i < SAMPLES; i++) 
    diff_bank_latency[i] = measure_bank_latency(addr_A_ptr, addr_C_ptr);

  //print_results(same_bank_latency, diff_bank_latency); 
  print_to_json(same_bank_latency, diff_bank_latency);

  return 0;
}

// Supporting functions for printing results in different formats
// Function "compare" is used in the priting functions and you do not need it
int compare(const void *p1, const void *p2) {
  uint64_t u1 = *(uint64_t *)p1;
  uint64_t u2 = *(uint64_t *)p2;

  return (int)u1 - (int)u2;
}

void print_results(uint64_t* same, uint64_t* diff) {

    qsort(same, SAMPLES, sizeof(uint64_t), compare);
    qsort(diff, SAMPLES, sizeof(uint64_t), compare);
    printf("             :  Same   Diff   \n");
    printf("Minimum      : %5ld %5ld\n", same[0], diff[0]);
    printf("Bottom decile: %5ld %5ld\n", same[SAMPLES/10], diff[SAMPLES/10]);
    printf("Median       : %5ld %5ld\n", same[SAMPLES/2], diff[SAMPLES/2]);
    printf("Top decile   : %5ld %5ld\n", same[(SAMPLES * 9)/10], diff[(SAMPLES * 9)/10]);
    printf("Maximum      : %5ld %5ld\n", same[SAMPLES-1], diff[SAMPLES-1]);
}

void print_to_json(uint64_t* same, uint64_t* diff)
{
      qsort(same, SAMPLES, sizeof(uint64_t), compare);
      qsort(diff,   SAMPLES, sizeof(uint64_t), compare);

      for (int i = 0; i < SAMPLES; i++) {
          printf("%lu ", same[i]);
      }
      printf("\n");
      for (int i = 0; i < SAMPLES; i++) {
          printf("%lu ", diff[i]);
      }
      printf("\n");
}
