#include <stdio.h>
#include <stdlib.h>
#include "avl_op.h"

/*
 * AVL insert operation implementation was referenced from www.geeksforgeeks.org/avl-tree-set-1-insertion/
 * searchNext function is not from the source but written by myself
 */

// Creates new node 
struct node *newNode(int time) {
	struct node *node = malloc(sizeof(struct node));
	node->time = time;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return node;
}

// Returns height of subtree N
int height(struct node *N) {
	// Height is set to 0 if node is NULL
	// This helps doing arithmetic on node with NULL values
	if(N == NULL)
		return 0;	

	return N->height;
}

// Returns maximum of two integers
int max(int a, int b) {
	if(a > b) 
		return a;
	else 
		return b;
}

// Performs a left rotate on subtree x
struct node *leftRotate(struct node *x) {
	struct node *y = x->right;
	struct node *T2 = y->left;

	// Peform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;

	// Return new root
	return y;
}

// Performs a right roate on subtree y
struct node *rightRotate(struct node *y) {
	struct node *x = y->left;
	struct node *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

	// Return new root
	return x;
}

// Returns balance factor for N
int getBalance(struct node *N) {
	if(N == NULL)
		return 0;
	return height(N->left) - height(N->right);	
}

// Performs AVL-tree insert
struct node* insert(struct node* node, int time) {

	// Perform the normal BST insertion
	if(node == NULL) 
		return newNode(time);

	if(time < node->time)
		node->left = insert(node->left, time);
	else
		node->right = insert(node->right, time);

	// Insertion is now done. We travel back.

	// Update height
	node->height = max(height(node->left), height(node->right)) + 1;

	// Obtain the balance factor for this node to check if subtree rooted 
	// at this node became unbalanced
	int balance = getBalance(node);

	// Check the balance factor and handle it accordingly.
	// There are 4 possible cases.

	// 1. Left Left Case
	// A signle right rotation on node balances it.
	if(balance > 1 && height(node->left) > height(node->right)) {
		return rightRotate(node);
	}

	// 2.Right Right Case
	// A single left roation on node balances it.
	if(balance < 1 && height(node->left) < height(node->right)) {
		return leftRotate(node);
	}

	// 3. Left Right Case
	// A double left-right rotation balances it.
	if(balance > 1 && height(node->left) < height(node->right)) {
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// 4. Right Left Case
	// A double right-left roatation balances it.
	if(balance < 1 && height(node->left) > height(node->right)) {
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

/* Given an integer x, it returns next smallest integer that is greater than x.
 * If there is no such integer, it returns -1.
 */
int searchNext(struct node *node, int x) {

	int next_int = -1;

	if(node == NULL) 
		return -1;
	
	// Move to right subtree if current node has value smaller than or equal
	// to x since we want value greater than x.
	if(node->time <= x) {
		next_int = searchNext(node->right, x);
	}

	// Move to left subtree if current node has value greater than x since
	// it is possible that there could be node with even smaller value (but still
	// greater than x) on left subtree.
	else {
		next_int = searchNext(node->left, x);
		
		// If next_int is -1, then there was no node with smaller value on
		// its left subtree.
		if(next_int == -1) {
			next_int = node->time;
		}
	}

	return next_int;
}
