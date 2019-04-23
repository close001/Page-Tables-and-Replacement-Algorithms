#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"

extern int memsize;

extern int debug;

extern struct frame *coremap;

extern int current_time;

/* Page to evict is chosen using the fifo algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int fifo_evict() {
	
	int i;
	// Records access time for page that came first
	int oldest = current_time; 
	// Stores frame number for victim page
	int victim;

	// Find page that came first.
	// Page that came first has the lowest access time.
	for(i = 0; i < memsize; i++) {
		if(coremap[i].time < oldest) {
			oldest = coremap[i].time;
			victim = i;
		}
	}
	return victim;
}

/* This function is called on each access to a page to update any information
 * needed by the fifo algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void fifo_ref(pgtbl_entry_t *p) {

	int frame = p->frame >> PAGE_SHIFT;

	// Update access time iff access to p was miss
	if(p->frame & PG_MISS) {
		coremap[frame].time = current_time;
	}
	current_time++;

	return;
}

/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void fifo_init() {

	int i;
	// Initialize time stamp for each page in the frame.
	// Note: time field was added to struct frame in pagetable.h
	for(i = 0; i < memsize; i++) { 
		coremap[i].time = -1;
	}

}
