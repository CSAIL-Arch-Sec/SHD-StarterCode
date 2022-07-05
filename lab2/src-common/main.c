/*
 * main
 * Initialization code for 6.888 Lab 2
 * Joseph Ravichandran Spring 2022
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/mman.h>

#include "lab2.h"
#include "lab2ipc.h"

/*
 * main
 * Setup shared memory and launch student code.
 */
int main(int argc, char *argv[]) {
    char *shared_memory;
    int kernel_fd;

    // Open a file descriptor to the kernel
    kernel_fd = open("/proc/lab2-victim", O_RDWR);
    if (kernel_fd < 0) {
        perror("Problem connecting to the kernel module- did you install it?\n");
        exit(EXIT_FAILURE);
    }

    // Create some shared memory that will be shared by both client and server
    shared_memory = mmap(NULL, LAB2_SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);

    if (NULL == shared_memory) {
        perror("mmap() error");
        exit(EXIT_FAILURE);
    }

    // Setup memory
    init_shared_memory(shared_memory, LAB2_SHARED_MEMORY_SIZE);

    // Run the attacker code :)
    return run_attacker(kernel_fd, shared_memory);
}
