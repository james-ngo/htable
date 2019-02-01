#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
	CharCode *codes;
	char *code;
	infile = fopen(argv[1], "r");
	if (!infile) {
		if (!argv[1]) 
			printf("usage: %s infile\n", argv[0]);
		else
			perror(argv[1]);
		return 1;
	}
	while (EOF != (c = fgetc(infile))) {
		if (!histogram[c]) {
			uniq_chars++;
		}
		histogram[c] += 1;
	}
	if (!uniq_chars) {
		return 0;
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
	to_tree(&list);
	i = 0;
	j = 0;
	code = (char*)malloc(sizeof(char));
	code[0] = '\0';
	codes = (CharCode*)malloc(uniq_chars * sizeof(CharCode));
	traverse(list.head, codes, code, i, &j);
	sort_codes(codes, uniq_chars);
	for (i = 0; i < uniq_chars; i++) {
		printf("0x%.2x: %s\n", codes[i].c, codes[i].code);
	}
	free(node_arr);
	free(code);
	for (i = 0; i < uniq_chars; i++) {
		free(codes[i].code);
	}
	free(codes);
	free_all(list.head);
	fclose(infile);
	return 0;
}

void sort_codes(CharCode *codes, int n) {
	int i;
	int j;
	CharCode temp;
	int lowest;
	for (i = 0; i < n - 1; i++) {
		lowest = i;
		for (j = i + 1; j < n; j++) {
			if (codes[j].c < codes[lowest].c) {
				lowest = j;
			}
		}
		temp = codes[lowest];
		codes[lowest] = codes[i];
		codes[i] = temp;
	}
}

void free_all(Node *node) {
	if (node->left != NULL) {
		free_all(node->left);
	}
	if (node->right != NULL) {
		free_all(node->right);
	}
	free(node);
}

void traverse(Node *node, CharCode *codes, char *code, int i, int *j) {
	if (node->left == NULL && node->right == NULL) {
		codes[*j].code = (char*)malloc((strlen(code) + 1) *
			sizeof(char));
		codes[*j].c = node->c;
		strcpy(codes[*j].code, code);
		(*j)++;
		return;
	}
	if (node->left != NULL) {
		code[i] = '0';
		code[i + 1] = '\0';
		traverse(node->left, codes, code, i + 1, j);
	}
	if (node->right != NULL) {
		code[i] = '1';
		code[i + 1] = '\0';
		traverse(node->right, codes, code, i + 1, j);
	}
}

void remove_at(Node *node_arr, int i, int n) {
	for (; i < n - 1; i++) {
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

void treeify(LinkedList *list) {
	Node *new_node = (Node*)malloc(sizeof(Node));
	new_node->c = '\0';
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
	if (node->freq < current_node->freq ||
		(node->freq == current_node->freq &&
		node->c < current_node->c)) {
		node->next = current_node;
		list->head = node;
		return;
	}
	while (current_node->next && (current_node->next->freq < node->freq ||
		(current_node->next->freq == node->freq &&
		current_node->next->c < node->c))) {
		current_node = current_node->next;
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
