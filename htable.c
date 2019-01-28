#include <stdio.h>
#include <stdlib.h>
#include "htable.h"

int main(int argc, char *argv[]) {
	FILE *infile;
	int histogram[256] = { 0 };
	int i;
	int c;
	int j;
	int uniq_chars = 0;
	int max_idx;
	Node *node_arr;
	Node *node;
	LinkedList list = { 0 };
	infile = fopen(argv[1], "r");
	while (EOF != (c = fgetc(infile))) {
		if (!histogram[c]) {
			uniq_chars++;
		}
		histogram[c] += 1;
	}
	node_arr = (Node*)malloc(uniq_chars * sizeof(Node));
	j = 0;
	for (i = 0; i < 256; i++) {
		if (histogram[i]) {
			node_arr[j].c = i;
			node_arr[j].freq = histogram[i];
			j++;
		}
	}
	max_idx = uniq_chars;
	for (i = 0; i < uniq_chars; i++) {
		node = (Node*)malloc(sizeof(Node));
		node->c = node_arr[min(node_arr, max_idx)].c;
		node->freq = node_arr[min(node_arr, max_idx)].freq;
		append(&list, node);
		remove_at(node_arr, min(node_arr, max_idx), max_idx);
		max_idx--;
	}
	printf("uniq_chars: %d\n", uniq_chars);
	print_list(&list);
	to_tree(&list);
	printInorder(list.head);
	return 0;
}



void print_list(LinkedList* list) {
	Node *current_node = list->head;
	while (current_node) {
		printf("%9c : %d\n", current_node->c, current_node->freq);
		current_node = current_node->next;
	}
}

void remove_at(Node *node_arr, int i, int n) {
	for (; i < n; i++) {
		node_arr[i] = node_arr[i + 1];
	}
}

int min(Node *node_arr, int n) {
	int i;
	int lo = 0;
	for (i = 1; i < n; i++) {
		if (node_arr[i].freq < node_arr[lo].freq ||
			(node_arr[i].freq == node_arr[lo].freq &&
			node_arr[i].c < node_arr[lo].c)) {
			lo = i;
		}
	}
	return lo;
}

void to_tree(LinkedList *list) {
	while (list->head->next) {
		treeify(list);
	}
}

void printInorder(Node* node) { 
	if (node == NULL) 
		return;
	printInorder(node->left); 
	printf("%9c : %d\n", (node->c == '\0'? ' ' : node->c), node->freq);
	printInorder(node->right); 
} 

void treeify(LinkedList *list) {
	Node *new_node = (Node*)malloc(sizeof(Node));
	new_node->left = list->head;
	new_node->freq = list->head->freq;
	list->head = list->head->next;
	new_node->right = list->head;
	new_node->freq += list->head->freq;
	list->head = list->head->next;
	insert(list, new_node);
}

void insert(LinkedList *list, Node *node) {
	Node *current_node = list->head;
	if (NULL == current_node) {
		append(list, node);
		return;
	}
	while (current_node->next && (current_node->next->freq < node->freq ||
		(current_node->next->freq == node->freq &&
		current_node->next->c < node->c))) {
		current_node = current_node->next;
		if (NULL == current_node) {
			append(list, node);
			return;
		}
	}
	node->next = current_node->next;
	current_node->next = node;
}

void append(LinkedList *list, Node *node) {
	if (NULL == list->head) {
		list->head = node;
		list->tail = node;
	}
	else {
		list->tail->next = node;
		list->tail = node;
	}
}
