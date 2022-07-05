#include "part3.hh"

/*
 * hammer_row
 *
 * Hammers a specific DRAM row
 *
 * Input: vict_row - The victim DRAM row index (not address!) to perform double-sided hammering on
 * Output: True if a bit flip is observed (anywhere in the victim row), false otherwise
 *
 */
bool hammer_row(uint64_t vict_row) {
  //TODO: Part 3

  return false;
}


int main(int argc, char** argv) {
  setvbuf(stdout, NULL, _IONBF, 0);

  if (getuid()) {
    printf("ERROR: Root permissions required!\n");
    exit(1);
  }
  
  allocated_mem = allocate_pages(BUFFER_SIZE_MB * 1024UL * 1024UL);
  
  setup_PPN_VPN_map(allocated_mem, PPN_VPN_map);

  //TODO: Part 3
}
