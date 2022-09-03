#ifndef PRACTICE_H
#define PRACTICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* define binary tree node structure */
typedef struct node {
    int leaf;
    char *questionData;
    struct node *left;
    struct node *right;
} node;

/* define stringArray used for collecting strings from database */
typedef struct stringArray {
    char** lines;
    int len;
} stringArray;


/* FUNCTION PROTOTYPES */
stringArray* collectLines (FILE *db);

char* collectLeafInfo (FILE *db, int size);

char *read_long_line(FILE *infile);

node* createNode(char* data);

void preorderTraversal(node *root);

node *construct(char **preorder, int preorder_size, char *isLeaf, int *pIndex);

node *constructTree(char **preorder, int preorder_size, char *isLeaf);

void addQuestion(node* curNode, char ans, char *anm, char *Q);

void printStrings(node *root, FILE *file);

void printLeafs(node *root, FILE *file);

void freeTree(node *root);

void freeSA(stringArray *sa);

void freeLA(char* la);

#endif
