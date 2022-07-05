/*
 * MIT 6.888 Lab 5: Address Space Layout Randomization
 * Part 1: Breaking ASLR.
 *
 * Created by Joseph Ravichandran
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

#include <assert.h>
#include <stddef.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <sys/prctl.h>
#include <sys/syscall.h>

#include "lab5.h"

extern uint64_t find_address(uint64_t low_bound, uint64_t high_bound);

uint64_t get_random_seed() {
	uint64_t randomness = 0;
	FILE *devrand = fopen("/dev/urandom", "rb");

	if (NULL == devrand) {
		perror("Error opening /dev/urandom");
		exit(EXIT_FAILURE);
	}

	size_t objects_read = fread(&randomness, sizeof(randomness), 1, devrand);
	if (1 != objects_read) {
		perror("Error reading from /dev/urandom");
		printf("Read %ld object(s)\n", objects_read);
		exit(EXIT_FAILURE);
	}

	randomness = randomness ^ getpid() ^ time(NULL);

	fclose(devrand);

	return randomness;
}

#define SECCOMP_DATA_SYSCALL_NUMBER ((offsetof(struct seccomp_data, nr)))

// See: https://www.kernel.org/doc/Documentation/networking/filter.txt
// See: https://man7.org/training/download/secisol_seccomp_slides.pdf
void setup_jail() {
	struct sock_filter filter[] = {
		// Load the number into the accumulator
		BPF_STMT(BPF_LD | BPF_W | BPF_ABS, SECCOMP_DATA_SYSCALL_NUMBER),

		// Compare against an allowed syscall
		// Jump if equal to the constant (BPF_K == constant)
		// Arg 3: number of lines to skip if condition is true
		// Arg 4: number of lines to skip if condition is false
		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_access, 0, 1),
		BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),

		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_fstat, 0, 1),
		BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),

		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_write, 0, 1),
		BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),

		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_exit, 0, 1),
		BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),

		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_close, 0, 1),
		BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),

		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_exit_group, 0, 1),
		BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),

		// By default, kill the process
		BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL_PROCESS),
	};

	struct sock_fprog program = {
		.len = sizeof(filter) / sizeof(filter[0]),
		.filter = filter,
	};

	// https://www.kernel.org/doc/Documentation/prctl/no_new_privs.txt
	if (-1 == prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
		perror("Error restricting privileges");
		exit(EXIT_FAILURE);
	}

	if (-1 == prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &program)) {
		perror("Error installing the seccomp-bpf filter");
		exit(EXIT_FAILURE);
	}
}

int main() {
	uint64_t lower_bound;
	uint64_t upper_bound;
	uint64_t seed = get_random_seed();
	srand(seed);

	uint64_t random_addr = (0x0000000FFFFFFFFF & rand()) << 12;
	char *shared_memory = mmap((void *)random_addr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);

	if (NULL == shared_memory) {
		perror("mmap() error\n");
		exit(EXIT_FAILURE);
	}

	setup_jail();

	shared_memory[0] = 'A';

	lower_bound = ((uint64_t)shared_memory) - PAGE_SIZE;
	upper_bound = ((uint64_t)shared_memory) + PAGE_SIZE;

	lower_bound -= (rand() % 500) * PAGE_SIZE;
	upper_bound += (rand() % 500) * PAGE_SIZE;

	uint64_t num_addrs = (upper_bound - lower_bound) / 0x1000;

	uint64_t their_guess = find_address(lower_bound, upper_bound);

	if (their_guess == (uint64_t)shared_memory) {
		printf("Correct!!!\n");
	}
	else {
		printf("Incorrect :(\n");
	}
	printf("It was: 0x%lX\nYou said: 0x%lX\n", (uint64_t)shared_memory, their_guess);

	return EXIT_SUCCESS;
}
