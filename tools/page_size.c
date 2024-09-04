// gcc -o page_size page_size.c

#include <unistd.h>
#include <stdio.h>

int main() {
    long page_size = sysconf(_SC_PAGESIZE);
    printf("System page size: %ld bytes\n", page_size);
    return 0;
}
