#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "BSTree.h"

/*Det Šr helt tillŒtet att lŠgga till egna hjŠlpfunktioner men de befintliga funktionerna fŒr inte Šndras*/

/* Statiska hjalpfunktioner anvands av andra funktioner i tradet och ska inte ligga i interfacet (anvandaren behover inte kanna till dessa) */

/*  KLAR  */ //TODO: REMOVE
int hasChildren(BSTree* tree) {
    if((*tree)->right == NULL && (*tree)->left == NULL) return 0;
    return 1;
}

/*  KLAR  */ //TODO: REMOVE
// Har inte return i alla paths?
BSTree * getSmallest(BSTree * tree) {
    if((*tree)->left != NULL) getSmallest(&(*tree)->left);
    else return tree;
}

/*  KLAR  */ //TODO: REMOVE
static struct treeNode* createNode(int data)
{
    struct treeNode * T = malloc(sizeof(struct treeNode));
    assert(T != NULL);
    if(T == NULL) return NULL;
    T->data = data;
    T->left = NULL;
    T->right = NULL;
    return T;
}

/*  KLAR  */ //TODO: REMOVE
void writeToArray(const BSTree tree, int * array, int length) {
    if(isEmpty(tree)) return;
    BSTree * pTree = &tree;
    pTree = getSmallest(pTree);
    if(tree->data > (*pTree)->data) {
        array[length] = (*pTree)->data;
        removeElement(pTree, (*pTree)->data);
    }
    else {
        array[length] = tree->data;
        removeElement(&tree, tree->data);
    }
    writeToArray(tree, array, length+1);
}

static int* writeSortedToArray(const BSTree tree)
{
    int * T = malloc(sizeof(int)*numberOfNodes(tree));
    assert(T != NULL);
    if(T == NULL) return NULL;
    writeToArray(tree, T, 0);
    return T;
}

/*  KLAR  */ //TODO: REMOVE
// Har fel på datat i arrayen efter.
struct treeNode * build(const int arr[], int start, int end) {
    if(start > end) return NULL;
    int mid = (start + end) / 2;
    struct treeNode * node = createNode(arr[mid]);
    node->left = build(arr, start, mid-1);
    node->right = build(arr, mid+1, end);
    return node;
}

/*  KLAR  */ //TODO: REMOVE
// Har fel på datat i arrayen efter.
static void buildTreeSortedFromArray(BSTree* tree, const int arr[], int size)
{
    //printf_s("Array before sorting.\n");     //TODO: REMOVE
    //for(int i = 0; i < size; i++) {          //TODO: REMOVE
    //    printf_s("\nValue: (%d)", arr[i]);   //TODO: REMOVE
    //}                                        //TODO: REMOVE
    //printf_s("\nEnd of array.\n");           //TODO: REMOVE
    struct treeNode * builtTree = build(arr, 0, size-1);
    *tree = builtTree;
}

/*  KLAR  */ //TODO: REMOVE
BSTree emptyTree(void)
{
    return NULL;
}

/*  KLAR  */ //TODO: REMOVE
int isEmpty(const BSTree tree)
{
    return tree == NULL ? 1 : 0;
}

/*  KLAR  */ //TODO: REMOVE
void insertSorted(BSTree* tree, int data)
{
    if(*tree == NULL) {
        (*tree) = createNode(data);
        return;
    }
    if(find(*tree, data) == 0) {
        if((*tree)->data > data) insertSorted(&(*tree)->right, data);
        else insertSorted(&(*tree)->left, data);
    }
    assert(find(*tree, data));
}

/*  KLAR  */ //TODO: REMOVE
void printPreorder(const BSTree tree, FILE *textfile)
{
    if(isEmpty(tree)) return;
    fprintf(textfile, "Node data: %d\n", tree->data);
    if(tree->left != NULL) printPreorder(tree->left, stdout);
    if(tree->right != NULL) printPreorder(tree->right, stdout);
}

/*  KLAR  */ //TODO: REMOVE
void printInorder(const BSTree tree, FILE *textfile)
{
    if(isEmpty(tree)) return;
    if(tree->left != NULL) printPreorder(tree->left, stdout);
    fprintf(textfile, "Node data: %d\n", tree->data);
    if(tree->right != NULL) printPreorder(tree->right, stdout);
}

/*  KLAR  */ //TODO: REMOVE
void printPostorder(const BSTree tree, FILE *textfile)
{
    if(isEmpty(tree)) return;
    if(tree->left != NULL) printPreorder(tree->left, stdout);
    if(tree->right != NULL) printPreorder(tree->right, stdout);
    fprintf(textfile, "Node data: %d\n", tree->data);
}

/*  KLAR  */ //TODO: REMOVE
// Har inte return i alla paths?
int find(const BSTree tree, int data)
{
    if(isEmpty(tree)) return 0;
    if(tree == NULL) return 0;
    if(tree->data == data) return 1;
    if(tree->data > data) find(tree->right, data);
    else find(tree->left, data);
}

/*  KLAR  */ //TODO: REMOVE
// Trädet är speglat?
void removeElement(BSTree* tree, int data)
{
    if(isEmpty(*tree)) return;
    if((*tree)->data == data) {
        if(!hasChildren(&(*tree))) {
            //printf("\nI am removing 1 node with 0 children, My current value is (%d)\n", (*tree)->data); //TODO: REMOVE
            (*tree)->right = NULL;
            (*tree)->left = NULL;
            free(*tree);
            *tree = NULL;
        }
        else {
            if((*tree)->right != NULL && (*tree)->left != NULL) {
                //printf("\nI am removing 1 node with 2 children (Left %d, Right %d), My current value is (%d)\n", (*tree)->left->data, (*tree)->right->data , (*tree)->data); //TODO: REMOVE
                BSTree * toRemove = getSmallest(&(*tree)->right);
                (*tree)->data = (*toRemove)->data;
                free(*toRemove);
                *toRemove = NULL;
            }
            else if((*tree)->left == NULL) {
                //printf("\nI am removing 1 node with 1 child (Right %d), My current value is (%d)\n", (*tree)->right->data , (*tree)->data); //TODO: REMOVE
                BSTree remove = *tree;
                *tree = (*tree)->right;
                free(remove);
            }
            else {
                //printf("\nI am removing 1 node with 1 child (Left %d), My current value is (%d)\n", (*tree)->left->data, (*tree)->data); //TODO: REMOVE
                BSTree remove = *tree;
                *tree = (*tree)->left;
                free(remove);
            }
        }
    }
    else {
        if((*tree)->data > data) removeElement(&(*tree)->right, data);
        else removeElement(&(*tree)->left, data);
    }
}

/*  KLAR  */ //TODO: REMOVE
// isEmpty & tree != NULL gör samma sak?
int numberOfNodes(const BSTree tree)
{
    if(isEmpty(tree)) return 0;
    if(tree != NULL) return numberOfNodes(tree->left) + numberOfNodes(tree->right) + 1;
    else return 0;
}

/*  KLAR  */ //TODO: REMOVE
int depth(const BSTree tree)
{
    if(isEmpty(tree)) return 0;
    int left = depth(tree->left);
    int right = depth(tree->right);
    return max(left, right) + 1;
}

/*  KLAR  */ //TODO: REMOVE
int minDepth(const BSTree tree)
{
    if(isEmpty(tree)) return 0;
    int nodes = numberOfNodes(tree) + 1;
    double calc = log2((double) nodes);
    return ceil(calc);
}

/*  KLAR  */ //TODO: REMOVE
void balanceTree(BSTree* tree)
{
    if(isEmpty(*tree)) return;
    int length = numberOfNodes(*tree);
    int * array = writeSortedToArray(*tree);
    buildTreeSortedFromArray(tree, array, length);
}

/*  KLAR  */ //TODO: REMOVE
void freeTree(BSTree* tree)
{
    if(isEmpty(*tree)) return;
    while(!isEmpty(*tree)) removeElement(tree, (*tree)->data);
    assert(isEmpty(*tree));
}


