#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "BSTree.h"

int hasChildren(BSTree* tree) {
    if((*tree)->right == NULL && (*tree)->left == NULL) return 0;
    return 1;
}

BSTree * getSmallest(BSTree * tree) {
    if((*tree)->left != NULL) return getSmallest(&(*tree)->left);
    return tree;
}

static struct treeNode* createNode(int data)
{
    struct treeNode * T = malloc(sizeof(struct treeNode));
    assert(T != NULL);
    if(T == NULL) abort();
    T->data = data;
    T->left = NULL;
    T->right = NULL;
    return T;
}

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

struct treeNode * build(const int arr[], int start, int end) {
    if(start > end) return NULL;
    int mid = (start + end) / 2;
    struct treeNode * node = createNode(arr[mid]);
    node->left = build(arr, start, mid-1);
    node->right = build(arr, mid+1, end);
    return node;
}

static void buildTreeSortedFromArray(BSTree* tree, const int arr[], int size)
{
    struct treeNode * builtTree = build(arr, 0, size-1);
    *tree = builtTree;
}

BSTree emptyTree(void)
{
    return NULL;
}

int isEmpty(const BSTree tree)
{
    return tree == NULL ? 1 : 0;
}

void insertSorted(BSTree* tree, int data)
{
    if(*tree == NULL) {
        (*tree) = createNode(data);
        return;
    }
    if(find(*tree, data) == 0) {
        if((*tree)->data > data) insertSorted(&(*tree)->left, data);
        else insertSorted(&(*tree)->right, data);
    }
    assert(find(*tree, data));
}

void printPreorder(const BSTree tree, FILE *textfile)
{
    if(isEmpty(tree)) return;
    if(tree == NULL) return;
    fprintf(textfile, "Node data: %d\n", tree->data);
    printPreorder(tree->left, stdout);
    printPreorder(tree->right, stdout);
}

void printInorder(const BSTree tree, FILE *textfile)
{
    if(isEmpty(tree)) return;
    if(tree == NULL) return;
    printInorder(tree->left, stdout);
    fprintf(textfile, "Node data: %d\n", tree->data);
    printInorder(tree->right, stdout);
}

void printPostorder(const BSTree tree, FILE *textfile)
{
    if(isEmpty(tree)) return;
    if(tree == NULL) return;
    printPostorder(tree->left, stdout);
    printPostorder(tree->right, stdout);
    fprintf(textfile, "Node data: %d\n", tree->data);
}

int find(const BSTree tree, int data)
{
    if(isEmpty(tree)) return 0;
    if(tree->data == data) return 1;
    if(tree->data > data) return find(tree->left, data);
    return find(tree->right, data);
}

void removeElement(BSTree* tree, int data)
{
    BSTree* topView = tree;
    if(isEmpty(*tree)) return;
    if((*tree)->data == data) {
        if(!hasChildren(&(*tree))) {
            (*tree)->right = NULL;
            (*tree)->left = NULL;
            free(*tree);
            *tree = NULL;
        }
        else {
            if((*tree)->right != NULL && (*tree)->left != NULL) {
                BSTree * toRemove = getSmallest(&(*tree)->right);
                int temp = (*toRemove)->data;
                removeElement(topView, temp);
                (*tree)->data = temp;
            }
            else if((*tree)->left == NULL) {
                BSTree remove = *tree;
                *tree = (*tree)->right;
                free(remove);
            }
            else {
                BSTree remove = *tree;
                *tree = (*tree)->left;
                free(remove);
            }
        }
    }
    else {
        if((*tree)->data > data) removeElement(&(*tree)->left, data);
        else removeElement(&(*tree)->right, data);
    }
}

int numberOfNodes(const BSTree tree)
{
    if(tree != NULL) return numberOfNodes(tree->left) + numberOfNodes(tree->right) + 1;
    else return 0;
}

int depth(const BSTree tree)
{
    if(isEmpty(tree)) return 0;
    int left = depth(tree->left);
    int right = depth(tree->right);
    return max(left, right) + 1;
}

int minDepth(const BSTree tree)
{
    if(isEmpty(tree)) return 0;
    int nodes = numberOfNodes(tree) + 1;
    double calc = log2((double) nodes);
    return ceil(calc);
}

void balanceTree(BSTree* tree)
{
    if(isEmpty(*tree)) return;
    int length = numberOfNodes(*tree);
    int * array = writeSortedToArray(*tree);
    buildTreeSortedFromArray(tree, array, length);
    free(array);
}

void freeTree(BSTree* tree)
{
    if(isEmpty(*tree)) return;
    while(!isEmpty(*tree)) removeElement(tree, (*tree)->data);
    assert(isEmpty(*tree));
}


