#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define HPAGE_SIZE (2 * 1024 * 1024)

int main(int argc, char ** argv) {
    char cmd[30];
    pid_t pid = getpid();
    
	
    size_t size = 32 * HPAGE_SIZE;
    
    void *mem = mmap(NULL, size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    madvise(mem, size, MADV_HUGEPAGE);

    memset(mem, 0, size);

    printf("Allocated %zu bytes at %p\n", size, mem);
    
    // print how pages are allocated
    sprintf(cmd, "cat /proc/%d/smaps_rollup", pid);
    system(cmd);

    munmap(mem, size);
    return 0;
}

