/*
 * MIT 6.888 Lab 5: Address Space Layout Randomization
 * Part 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <mmintrin.h>
#include <xmmintrin.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <x86intrin.h>
#include "lab5.h"

// Same as in Part 2
extern void vulnerable(char *your_string);
extern void call_me_maybe(uint64_t rdi, uint64_t rsi, uint64_t rdx);

// Your code:
uint64_t find_address(uint64_t low_bound, uint64_t high_bound);
void do_overflow(uint64_t page_addr);

uint64_t find_address(uint64_t low_bound, uint64_t high_bound) {

	// Put your Part 1 code here
	// You are free to choose any of 1A, 1B, or 1C

    return 0;
}

/*
 * do_overflow
 * Construct the ROP chain and execute it using the gadgets we found by breaking ASLR.
 */
void do_overflow(uint64_t page_addr) {

	// Put your Part 2 code here
	// Use the page you found with find_address
	// and the offsets located from objdump to find your gadgets

}

/*
 * lab5_code
 * This is called by main with the bounds for the hidden page, just
 * like in Part 1. You will locate the page and then execute a ROP
 * chain using payloads located in the page.
 */
void lab5_code(uint64_t low_bound, uint64_t high_bound) {
	uint64_t found_page = find_address(low_bound, high_bound);
	do_overflow(found_page);
}
