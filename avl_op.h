#ifndef __AVL_OP_H__
#define __AVL_OP_H__

struct node {
	int time;   // Records access time
	struct node *left;
	struct node *right;
	int height;
};

struct node *newNode(int time);
int height(struct node *N);
int max(int a, int b);
struct node *leftRotate(struct node *x);
struct node *rightRotate(struct node *y);
int getBalance(struct node *N);
struct node* insert(struct node *node, int time);
int searchNext(struct node *node, int x);


#endif /* AVL_OP_H */
