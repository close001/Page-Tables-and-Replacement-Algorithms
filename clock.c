#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"

extern int memsize;

extern int debug;

extern struct frame *coremap;

// Stores frame number that clock hand currenlty points to.
int hand = 0;

/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int clock_evict() {

	// Determine which page to evict
	while(coremap[hand].pte->frame & PG_REF) {
		coremap[hand].pte->frame &= ~PG_REF;
		hand = (hand + 1) % memsize;
	}
	
	// Clock hand now points to the first frame whose page has ref bit == 0.

	return hand;
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {

	// Move clock hand if access to p was miss.
	// On hit, Clock hand remains where it is at.
	if(p->frame & PG_MISS) {
		hand = (hand + 1) % memsize;
	}

	// Set ref bit to high
	p->frame = p->frame | PG_REF;
	return;
}

/* Initialize any data structures needed for this replacement
 * algorithm. 
 */
void clock_init() {

}
