#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "sim.h"
#include "pagetable.h"
#include "avl_op.h"


/* All AVL-tree operations, insert and searchNext, are in avl_op.c */


extern unsigned int memsize;

extern int debug;

extern struct frame *coremap;

extern char *tracefile;

// Stores a pointer to each page's access time record
struct node **accessbook;


// Reads tracefile and records access time for each page
void record_time() {

	char buf[MAXLINE];
	addr_t vaddr = 0;
	char type;
	FILE *tfp;
	int current_time = 0;
	// Index for accessbook (top 24 (or 20) bits in vaddar are used to 
	// determine entry for each unique page)
	int idx;
	
	// Open tracefile
	if(tracefile != NULL) {
		if((tfp = fopen(tracefile, "r")) == NULL) {
			perror("Error opening tracefile:");
			exit(1);
		}
	}

	// Read each data access and save its access time to 
	// corresponding page's data structure
	while(fgets(buf, MAXLINE, tfp) != NULL) {
		// For the test files
		if(buf[0] != '=') {
			sscanf(buf, "%c %lx", &type, &vaddr);
			if(debug) {
				printf("%c %lx\n", type, vaddr);
			}
			// Virtual address is used to index into accessbook
			idx = vaddr >> PAGE_SHIFT;
			accessbook[idx] = insert(accessbook[idx], current_time);
		} else {
			continue;
		}

		current_time++;
	}
}

// Gets virtual address for page in idx_th frame in physmem
addr_t getVaddr(int idx) {
	char *mem_ptr = &physmem[idx*SIMPAGESIZE];
	addr_t *vaddr = (addr_t *)(mem_ptr + sizeof(int));
	return *vaddr;
}


/* Page to evict is chosen using the optimal (aka MIN) algorithm. 
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int opt_evict() {

	int i;
	// Frame number for victim page
	int victim = -1;
	// Current time
	int current_time = ref_count;
	// Records longest time gap from current time
	int longest = -1;
	// Upcoming access time
	int next_access = -1;
	// Index for accessbook
	int idx;
	addr_t vaddr;
	struct node *node;

	// Find the page in physmem whose gap between current time
	// and its upcoming access time is longest
	for(i = 0; i < memsize; i++) {
	
		// Get vaddr for current page
		vaddr = getVaddr(i);

		// Find upcoming access time for current page
		idx = vaddr >> PAGE_SHIFT;
		node = accessbook[idx];
		next_access = searchNext(node, current_time);

		// Choose current page as victim if it is never accessed again
		if(next_access == -1) {
			victim = i;
			break;
		}

		// Compare current page's upcoming access time with longest
		// and update longest and victim if needed
		if(next_access > longest) {
			longest = next_access; 
			victim = i;
		}

	}	
	return victim;
}

/* This function is called on each access to a page to update any information
 * needed by the opt algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void opt_ref(pgtbl_entry_t *p) {

	return;
}

/* Initializes any data structures needed for this
 * replacement algorithm.
 */
void opt_init() {

	int i;
	// size of accessbook 
	int booksize = PTRS_PER_PGDIR * PTRS_PER_PGTBL;

	// Initialize accessbook
	accessbook = malloc(sizeof(struct node *) * booksize);
	for(i = 0; i < booksize; i++) {
		accessbook[i] = NULL;
	}

	// Read tracefile and record access time
	// Each page gets data structure to store its access time
	record_time();	

}

