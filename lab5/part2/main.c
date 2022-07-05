/*
 * MIT 6.888 Lab 5: Address Space Layout Randomization
 * Part 2: ret2win and ROP.
 *
 * Created by Joseph Ravichandran
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "lab5.h"

// User code
void lab5_code();

// Don't use the stack to store the cursor that is used to print the stack!
static int LOOP_CURSOR = 0;


#ifndef PART2B

#define LOOP_ITERS ((3))

/*
 * win
 * Call this method to win!
 */
void win() {
    puts("Congrats, you win!!\n");
    puts("MIT{nic3_s0lv3!}\n");
    exit(0);
}

#else // !PART2B

#define LOOP_ITERS ((16))

/*
 * call_me_maybe
 * Call this correctly to win
 */
void call_me_maybe(uint64_t rdi, uint64_t rsi, uint64_t rdx) {
    if ((rdi & 0x02) != 0) {
        if (rsi == 2 * rdi) {
            if (rdx == 1337) {
                printf("MIT{lab5_p4rtTw0_1s_d0n3}\n");
                exit(0);
            }
        }
    }
    printf("Incorrect arguments!\n");
    printf("You did call_me_maybe(0x%lX, 0x%lX, 0x%lX);\n", rdi, rsi, rdx);
    exit(-1);
}

#endif // PART2B

/*
 * my_strcpy
 * A version of strcpy that allows NULLs but stops on newlines ('\n' == 0x0A)
 */
void my_strcpy(char *dst, char *src) {
    char *cursor = src;

    while (*cursor != '\n') {
        *dst = *cursor;
        cursor++;
        dst++;
    }
}

/*
 * vulnerable
 * This method is vulnerable to a buffer overflow
 */
void vulnerable(char *your_string) {
    // Create 16 bytes on the stack
    char stackbuf[16];

    /*
    This is what the stack looks like:
    +-----------------------+
    | stackbuf bytes 0 to 7 |
    +-----------------------+
    | stackbuf bytes 8 to 15|
    +-----------------------+
    |       saved rbp       |
    +-----------------------+
    |    return address     | -> Points to wherever we are called from
    +-----------------------+
    */

    /*
    Recall that C strings are arrays of bytes.
    The pointer "your_string" points to the string you supplied.

    If you called vulnerable("Test\n"), memory looks like this:

    +-----+
    | 'T' | <- "your_string" points to the first character
    +-----+
    | 'e' |
    +-----+
    | 's' |
    +-----+
    | 't' |
    +-----+
    | 0xA | <- A newline
    +-----+
    | 0x0 | <- NULL Terminator
    +-----+

    We are going to copy your string into the "stackbuf" local variable, byte by byte.
    We use a cursor that copies byte by byte until it finds a new line in the input string.
    (See my_strcpy above).

    We allow copying NULL bytes to make your exploitation easier.

    your_string:                     stackbuf (empty):
    +-----+                          +-----+
    | 'T' | <- cursor starts here -> | 0x0 |
    +-----+                          +-----+
    | 'e' |                          | 0x0 |
    +-----+                          +-----+
    | 's' |                          | 0x0 |
    +-----+                          +-----+
    | 't' |                          | 0x0 |
    +-----+                          +-----+
    | 0xA |                          | 0x0 |
    +-----+                          +-----+
    | 0x0 |                          | 0x0 |
    +-----+                          +-----+

    your_string:                     stackbuf:
    +-----+                          +-----+
    | 'T' |    ----> copied ----->   | 'T' |
    +-----+                          +-----+
    | 'e' |  <- cursor goes here ->  | 0x0 |
    +-----+                          +-----+
    | 's' |                          | 0x0 |
    +-----+                          +-----+
    | 't' |                          | 0x0 |
    +-----+                          +-----+
    | 0xA |                          | 0x0 |
    +-----+                          +-----+
    | 0x0 |                          | 0x0 |
    +-----+                          +-----+

    your_string:                     stackbuf (empty):
    +-----+                          +-----+
    | 'T' |                          | 'T' |
    +-----+                          +-----+
    | 'e' |    ----> copied ----->   | 'e' |
    +-----+                          +-----+
    | 's' |  <- cursor goes here ->  | 0x0 |
    +-----+                          +-----+
    | 't' |                          | 0x0 |
    +-----+                          +-----+
    | 0xA |                          | 0x0 |
    +-----+                          +-----+
    | 0x0 |                          | 0x0 |
    +-----+                          +-----+

    Until it looks like this:

    your_string:                     stackbuf (empty):
    +-----+                          +-----+
    | 'T' |                          | 'T' |
    +-----+                          +-----+
    | 'e' |                          | 'e' |
    +-----+                          +-----+
    | 's' |                          | 's' |
    +-----+                          +-----+
    | 't' |                          | 't' |
    +-----+                          +-----+
    | 0xA |  ---> We stop here --->  | 0x0 |
    +-----+                          +-----+
    | 0x0 |                          | 0x0 |
    +-----+                          +-----+

    Where the last newline is not copied into the buffer
    (again, read my_strcpy to see why!)
    */

    // Perform the overflow
    my_strcpy(stackbuf, your_string);

    // Print the stack in chunks of 8 bytes:
#ifndef PART2B
    printf("\nRecall win() is at: 0x%lX\n", (uint64_t)&win);
#endif // !PART2B
    printf("\nThis is what the stack looks like now:\n");
    for (LOOP_CURSOR = 0; LOOP_CURSOR <= LOOP_ITERS; LOOP_CURSOR++) {
        printf("      +-----------------------------------------+\n");
        printf("0x%02X: | 0x%016lX = 0x%016lX |", LOOP_CURSOR, (uint64_t)&((uint64_t *)stackbuf)[LOOP_CURSOR], ((volatile uint64_t *)stackbuf)[LOOP_CURSOR]);

        if (LOOP_CURSOR == 0) {
            printf(" <- stackbuf starts here\n");
        }
        else if (LOOP_CURSOR == 2) {
            printf(" <- Saved RBP\n");
        }
        else if (LOOP_CURSOR == 3) {
#ifndef PART2B
            printf(" <- Return address!\n");
#else // !PART2B
            printf(" <- First return address\n");
#endif // PART2B
        }
        else {
            printf("\n");
        }
    }
    printf("      +-----------------------------------------+\n");
}

/*
 * main
 * Calls the user's lab 5 part 2 code.
 */
int main () {
    lab5_code();
}
