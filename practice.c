#include "practice.h"

/* CONSTANTS */
#define BUFSIZE 100

/* create node function */
node* createNode(char* data)
{
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node->questionData = data;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

/* updates the binary tree with new node information */
void addQuestion(node* curNode, char ans, char *anm, char *Q){
    /* left node is new answer, right node is wrong guess */
    if (ans == 'y' || ans == 'Y'){
        node *newLeft = createNode(anm);
        curNode->left = newLeft;
        node *newRight = createNode(curNode->questionData);
        curNode->right = newRight;
        curNode->questionData = Q;
    }
    /* right node is new answer, left node is wrong guess */
    else{
        node *newLeft = createNode(curNode->questionData);
        curNode->left = newLeft;
        node *newRight = createNode(anm);
        curNode->right = newRight;
        curNode->questionData = Q;
    }
}

/* Function to print the preorder traversal on a given binary tree */
void preorderTraversal(node *root) {
    if (root == NULL) {
        return;
    }

    printf("%s ", root->questionData);
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

/* print the question and answer data strings to database */
void printStrings(node *root, FILE *file) {
    if (root == NULL) {
        return;
    }

    fputs(root->questionData, file);
    fputc('\n', file);
    printStrings(root->left, file);
    printStrings(root->right, file);
}

/* print the leaf nodes to database */
void printLeafs(node *root, FILE *file) {
    if (root == NULL) {
        return;
    }

    if (root->left == NULL && root->right == NULL){
        fputc('1', file);
    }
    else{
        fputc('0', file);
    }
    printLeafs(root->left, file);
    printLeafs(root->right, file);
}

/* free tree */
void freeTree(node *root){
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root->questionData);
    free(root);
}

void freeSA(stringArray *sa){
    free(sa->lines);
    free(sa);
}

void freeLA(char* la){
    free(la);
}

/* HELPER - construct a full binary tree recursively given preorder traversal and boolean leaf node info */
node *construct(char **preorder, int preorderSize, char *leaf, int *pIndex) {
    /* base case */
    if (*pIndex == preorderSize) {
        return NULL;
    }

    /* printf("pIndex: %d\n", *pIndex); */
    node *node = createNode(preorder[*pIndex]);
    int isInternalNode = !leaf[*pIndex];
    (*pIndex)++;

    /* if cur node is not leaf, construct its 2 children */
    if (isInternalNode) {
        node->left = construct(preorder, preorderSize, leaf, pIndex);
        node->right = construct(preorder, preorderSize, leaf, pIndex);
    }

    return node;
}

/* construct a full binary tree recursively given preorder traversal and boolean leaf node info */
node *constructTree(char **preorder, int preorderSize, char *leaf) {
    /* variable stores the index of the next unprocessed key in a preorder sequence, start initialized at 0 */ 
    int pIndex = 0;

    return construct(preorder, preorderSize, leaf, &pIndex);
}



/* function to read in the question data of binary tree stored in database */
stringArray* collectLines (FILE *db){
    char *line;
    int size;
    stringArray *sa = malloc(sizeof(stringArray));

    size = BUFSIZE;

    /* error check */
    if (!(sa->lines = malloc(size * sizeof(char *)))){
       perror("malloc");
       exit(EXIT_FAILURE);
    }
    sa->len = 0;
    line = read_long_line(db);
    if (line != NULL){
        /* iterate until flag */
        while ((strcmp(line, "\\end"))){
            if (sa->len > size - 1){
                size += BUFSIZE;
                sa->lines = realloc(sa->lines, size);
                /* error check */
                if (sa->lines == NULL){
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
            }
            sa->lines[sa->len++] = line;
            line = read_long_line(db);
        }
        free(line);
        return sa;
    }
    else {
        return NULL;
    }
    
}


/* function to read in the leaf node information of binary tree stored in database */
char* collectLeafInfo (FILE *db, int size){
    char *la;
    int i = 0;

    la = malloc(size);
    if (la == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    while (i < size){
        la[i++] = getc(db) - '0';
    }

    return la;
}



/* read in a line from a file */ 
char *read_long_line(FILE *infile) {
   int c;
   int len;
   int size;
   char *s;

    /* error check */
   if (!(s = malloc(BUFSIZE))){
       perror("malloc");
       exit(EXIT_FAILURE);
   }
   len = 0;
   size = BUFSIZE;
   while (((c = getc(infile)) != '\n' && c != EOF)){
       /* expand */
       if (len > size - 2){
           size += BUFSIZE;
           s = realloc(s, size);
           if (s == NULL){
               perror("realloc");
               exit(EXIT_FAILURE);
           }
       }
       s[len++] = c;
   }
   s[len] = '\0';
   if (c == EOF && len == 0){
       free(s);
       s = NULL;
   }
   return s;

}
