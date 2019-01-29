#ifndef HTABLE_H
#define HTABLE_H

typedef struct node Node;

typedef struct linkedlist LinkedList;

typedef struct code CharCode;

struct node {
	int c;
	int freq;
	Node *next;
	Node *left;
	Node *right;
};

struct linkedlist {
	Node *head;
	Node *tail;
};

struct code {
	char c;
	char *code;
};

void traverse(Node*, char*, int);

void print_list(LinkedList*);

void printInorder(Node*);

void append(LinkedList*, Node*);

void insert(LinkedList*, Node*);

int min(Node*, int);

void remove_at(Node*, int, int);

void treeify(LinkedList*);

void to_tree(LinkedList*);

#endif
