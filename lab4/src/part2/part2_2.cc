#include "../shared.hh"
#include "../params.hh"

#define POOL_SIZE 1000
#define NUM_BINS 16

// TODO: Change this!
#define THRESHOLD 0

int main (int ac, char **av) {
  
  setvbuf(stdout, NULL, _IONBF, 0);

  if (getuid()) {
    printf("ERROR: Root permissions required!\n");
    exit(1);
  }

  // Allocate a large pool of memory (of size BUFFER_SIZE_MB) pointed to by allocated_mem
  allocated_mem = allocate_pages(BUFFER_SIZE_MB * 1024UL * 1024UL);

  //TODO: Part 2.2

  // Compute Consistency Rate
  // For each candidate:
  for (int cand = 0; cand < 3; cand++) {
    printf("===== Candidate Function: %d =====\n", cand);

    // And for each bin:
    for (int bin = 0; bin < NUM_BINS; bin++) {
      
      //TODO: Part 2.2
      
      double consistent_rate = 0;
      printf("Consistency Rate: %lf\n", consistent_rate);
    }
  }

  return 0;
}

