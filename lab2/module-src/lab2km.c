/*
 * lab2km.c
 * The kernel module for MIT 6.888 Lab 2 Spring 2022
 * Created by Joseph Ravichandran for Spring 2022
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/highmem.h>

#include "lab2km.h"
#include "lab2ipc.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joseph Ravichandran <jravi@csail.mit.edu>");
MODULE_DESCRIPTION("Lab 2 target module for 6.888 Spring 2022");

// The secrets you're trying to leak!
static volatile char __attribute__((aligned(32768))) kernel_secret3[LAB2_SECRET_MAX_LEN] = "MIT{h4rd3st}";
static volatile char __attribute__((aligned(32768))) kernel_secret2[LAB2_SECRET_MAX_LEN] = "MIT{scary_sp3ctr3!}";
static volatile char __attribute__((aligned(32768))) kernel_secret1[LAB2_SECRET_MAX_LEN] = "MIT{k3rn3l_m3m0r135}";

// The bounds checks that you need to use speculative execution to bypass
static volatile size_t __attribute__((aligned(32768))) secret_leak_limit_part2 = 4;
static volatile size_t __attribute__((aligned(32768))) secret_leak_limit_part3 = 4;

static struct proc_dir_entry *lab2_procfs_victim = NULL;
static const struct file_operations  lab2_victim_ops = {
    .write = lab2_victim_write,
    .read = lab2_victim_read,
};

/*
 * print_cmd
 * Prints a nicely formatted command to stdout
 *
 * Arguments:
 *  - cmd: A pointer to a lab2_command object to the kernel log
 *
 * Returns: None
 * Side Effects: Prints to kernel log
 */
void print_cmd(lab2_command *cmd) {
    if (NULL == cmd) return;

    printk(KERN_INFO "Command at %p\n\tKind: %d\n\tArg 1: 0x%llX\n",
            cmd,
            cmd->kind,
            cmd->arg1);
}

/*
 * lab2_init
 * Installs the procfs handlers for communicating with this module.
 */
int lab2_init(void) {
    printk(KERN_INFO "[lab2] Hello, World\n");
    lab2_procfs_victim = proc_create("lab2-victim", 0, NULL, &lab2_victim_ops);
    return 0;
}

/*
 * lab2_fini
 * Cleans up the procfs handlers and cleanly removes the module from the kernel.
 */
void lab2_fini(void) {
    printk(KERN_INFO "[lab2] Goodbye!\n");
    proc_remove(lab2_procfs_victim);
}

/*
 * lab2_victim_read
 * procfs read handler that does nothing so that reading from /proc/lab2-victim
 * can actually return.
 */
ssize_t lab2_victim_read(struct file *file_in, char __user *userbuf, size_t num_bytes, loff_t *offset) {
    // Reading does nothing
    return 0;
}

/*
 * lab2_victim_write
 * procfs write handler for interacting with the module
 * Writes expect the user to write a lab2_command struct to the module.
 *
 * Input: A lab2_command struct for us to parse.
 * Output: Number of bytes accepted by the module.
 * Side Effects: Will trigger a spectre bug based on the user_cmd.kind
 */
ssize_t lab2_victim_write(struct file *file_in, const char __user *userbuf, size_t num_bytes, loff_t *offset) {
    lab2_command user_cmd;
    struct page *pages[LAB2_SHARED_MEMORY_NUM_PAGES];
    char *kernel_mapped_region[LAB2_SHARED_MEMORY_NUM_PAGES];
    int retval;
    int i, j, z;
    char secret_data;
    volatile char tmp;
    size_t long_latency;
    char *addr_to_leak;

    for (i = 0; i < LAB2_SHARED_MEMORY_NUM_PAGES; i++) {
        pages[i] = NULL;
    }

    if (copy_from_user(&user_cmd, userbuf, sizeof(user_cmd)) == 0) {
        // arg1 is always a pointer to the shared memory region
        if (!access_ok(user_cmd.arg1, LAB2_SHARED_MEMORY_SIZE)) {
            printk(KERN_INFO "[lab2] Invalid user request- shared memory is 0x%llX\n", user_cmd.arg1);
            return num_bytes;
        }

        // arg2 is always the secret index to use
        if (!(user_cmd.arg2 < LAB2_SECRET_MAX_LEN)) {
            printk(KERN_INFO "[lab2] Tried to access a secret that is too large! Requested offset %llu\n", user_cmd.arg2);
            return num_bytes;
        }

        // Pin the pages to RAM so they aren't swapped to disk
        retval = get_user_pages_fast(user_cmd.arg1, LAB2_SHARED_MEMORY_NUM_PAGES, FOLL_WRITE, pages);
        if (LAB2_SHARED_MEMORY_NUM_PAGES != retval) {
            printk(KERN_INFO "[lab2] Unable to pin the user pages! Requested %d pages, got %d\n", LAB2_SHARED_MEMORY_NUM_PAGES, retval);

            // If the return value is negative, its an error, so don't try to unpin!
            if (retval > 0) {
                // Unpin the pages that got pinned before exiting
                put_user_pages(pages, retval);
            }

            return num_bytes;
        }

        // Map the new pages (aliases to the userspace pages) into the kernel address space
        // Accessing these pages will incur a TLB miss as they were just remapped
        for (i = 0; i < LAB2_SHARED_MEMORY_NUM_PAGES; i++) {
            kernel_mapped_region[i] = (char *)kmap(pages[i]);

            if (NULL == kernel_mapped_region[i]) {
                printk(KERN_INFO "[lab2] Unable to map page %d\n", i);

                // Unmap everything in reverse order and return early
                for (j = i - 1; j >= 0; j--) {
                    kunmap(pages[j]);
                }

                return num_bytes;
            }
        }

        // Process this command packet
        switch (user_cmd.kind) {
            // Part 1 is Flush+Reload, so access a secret without a bounds check
            case COMMAND_PART1:
                secret_data = kernel_secret1[user_cmd.arg2];
                if (secret_data < LAB2_SHARED_MEMORY_NUM_PAGES) {
                    tmp = *kernel_mapped_region[secret_data];
                }
            break;

            // Part 2 is Spectre, so access a secret bounded by a bounds check
            case COMMAND_PART2:
                // Load the secret:
                secret_data = kernel_secret2[user_cmd.arg2];

                // Trigger a page walk:
                addr_to_leak = kernel_mapped_region[secret_data];

                // Flush the limit variable to make this if statement take a long time to resolve
                clflush(&secret_leak_limit_part2);
                if (user_cmd.arg2 < secret_leak_limit_part2) {
                    // Perform the speculative leak
                    tmp = *addr_to_leak;
                }
            break;

            // Part 3 is a more difficult version of Spectre
            case COMMAND_PART3:
                // No cache flush this time around!
                for (z = 0; z < 1000; z++);
                if (user_cmd.arg2 < secret_leak_limit_part3) {
                    long_latency = user_cmd.arg2 * 1ULL * 1ULL * 1ULL * 1ULL * 0ULL;
                    tmp = *kernel_mapped_region[kernel_secret3[user_cmd.arg2] + long_latency];
                }
            break;
        }

        // Unmap in reverse order- needs to be reverse order!
        for (i = LAB2_SHARED_MEMORY_NUM_PAGES - 1; i >= 0; i--) {
            kunmap(pages[i]);
        }

        // Unpin to ensure refcounts are valid
        put_user_pages(pages, LAB2_SHARED_MEMORY_NUM_PAGES);

        // Success!
        return num_bytes;
    }
    else {
        // Error
        return 0;
    }
}

module_init(lab2_init);
module_exit(lab2_fini);
