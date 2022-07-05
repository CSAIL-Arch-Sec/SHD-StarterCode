
#include "util.h"

/* Measure the time it takes to access a block with virtual address addr. */
CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile("mov %1, %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
	"mov (%%r8), %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"sub %%edi, %%eax\n\t"
	: "=a"(cycles) /*output*/
	: "r"(addr)
	: "r8", "edi");	

	return cycles;
}

/*
 * CLFlushes the given address.
 * 
 * Note: clflush is provided to help you debug and should not be used in your
 * final submission
 */
void clflush(ADDR_PTR addr)
{
    asm volatile ("clflush (%0)"::"r"(addr));
}

/*
 * Converts a string to its binary representation.
 */
char *string_to_binary(char *s)
{
  if (s == NULL)
    return 0; /* no input string */

  size_t len = strlen(s);

  // Each char is one byte (8 bits) and + 1 at the end for null terminator
  char *binary = malloc(len * 8 + 1);
  binary[len] = '\0';

  for (size_t i = 0; i < len; ++i)
  {
    char ch = s[i];
    for (int j = 7; j >= 0; --j)
    {
      if (ch & (1 << j))
      {
        strcat(binary, "1");
      }
      else
      {
        strcat(binary, "0");
      }
    }
  }

  return binary;
}

/*
 * Converts a binary string to its ASCII representation.
 */
char *binary_to_string(char *data)
{
  // Each char is 8 bits
  size_t msg_len = strlen(data) / 8;

  // Add one for null terminator at the end
  char *msg = malloc(msg_len + 1);
  msg[msg_len] = '\0';

  for (int i = 0; i < msg_len; i++)
  {
    char tmp[8];
    int k = 0;

    for (int j = i * 8; j < ((i + 1) * 8); j++)
    {
      tmp[k++] = data[j];
    }

    msg[i] = strtol(tmp, 0, 2);
  }

  return msg;
}

/*
 * Converts a string to integer
 */
int string_to_int(char* s) 
{
  return atoi(s);
}
