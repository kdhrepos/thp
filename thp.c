#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

#define HPAGE_SIZE (2 * 1024 * 1024)

int main(int argc, char ** argv) {
    char cmd[30];
    pid_t pid = getpid();
	
    int nr_huge_page; // number of huge pages

    if (argc < 2) {
	fprintf(stderr, "no args\n");
	return 1;
    }

    nr_huge_page = atoi(argv[1]);

    size_t size = nr_huge_page * HPAGE_SIZE;
    
    void *mem = mmap(NULL, size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    madvise(mem, size, MADV_HUGEPAGE);

    memset(mem, 1, size);

    printf("Allocated %zu bytes at %p\n", size, mem);
    printf("Huge page need to be allocated: %d\n", 2048 * nr_huge_page);

    // print how pages are allocated
    sprintf(cmd, "cat /proc/%d/smaps_rollup", pid);
    system(cmd);

    if (((uintptr_t)mem % HPAGE_SIZE) == 0) {
    	printf("mem (%p) is aligned to 2MB\n", mem);
    } else {
    	printf("mem (%p) is NOT aligned to 2MB\n", mem);
    }
    
    munmap(mem, size);
    return 0;
}

