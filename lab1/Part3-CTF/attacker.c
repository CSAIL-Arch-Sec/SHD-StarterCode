#include "util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>

int main(int argc, char const *argv[]) {
    int flag = -1;

    // Put your capture-the-flag code here

    printf("Flag: %d\n", flag);
    return 0;
}
