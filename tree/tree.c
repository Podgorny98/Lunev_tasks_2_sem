#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "tree.h"


Node* TreeCtor(int val) {
    Node* tree = Malloc(sizeof(Node));
    if(tree == NULL)
        return NULL;
    tree->data = val;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}
//============================================================
Node* AddNode(Node* tree, int val) {
    if(tree == NULL)
        return TreeCtor(val);
    if(val < tree->data) {
        if(tree->left == NULL) {
            tree->left = AddNode(tree->left, val);
            return tree->left;
        }
        return AddNode(tree->left, val);
    }
    if(val > tree->data) {
        if(tree->right == NULL) {
            tree->right = AddNode(tree->right, val);
            return tree->right;
        }
        return AddNode(tree->right, val);
    }
    return tree;
}
//============================================================
void TreeDtor(Node* tree) {
    if(tree == NULL)
        return;
    TreeDtor(tree->left);
    TreeDtor(tree->right);
    tree->data = 0;
    free(tree);
    tree = NULL;
}
//============================================================
int FindElement(Node* tree, int val) {
    if(tree == NULL)
        return 0;
    if(tree->data == val)
        return 1;
    if(val < tree->data)
        return FindElement(tree->left, val);
    return FindElement(tree->right, val);
}
//============================================================
int Visitor(Node* tree, enum TRAVERSAL_ORDER order, void (*func)(Node* node, void* ctx), void* ctx) {
    if(tree == NULL)
        return EFAULT;
    switch(order) {
        case PRE_ORDER:
            (*func)(tree, ctx);
            if(tree->left)
                Visitor(tree->left, order, (*func), ctx);
            if(tree->right)
                Visitor(tree->right, order, (*func), ctx);
            break;
        case IN_ORDER:
            if(tree->left)
                Visitor(tree->left, order, (*func), ctx);
            (*func)(tree, ctx);
            if(tree->right)
                Visitor(tree->right, order, (*func), ctx);
            break;
        case POST_ORDER:
            if(tree->left)
                Visitor(tree->left, order, (*func), ctx);
            if(tree->right)
                Visitor(tree->right, order, (*func), ctx);
            (*func)(tree, ctx);
            break;
        default:
            return EINVAL;
    }
    return 0;
}
//============================================================
int TreeDump(Node* tree, const char* pathname) {
    if(tree == NULL)
        return EFAULT;
    FILE* file = fopen(pathname, "w");
    if(file == NULL)
        return -1;
    int res = NodesDump(tree, file);
    fclose(file);
    file = NULL;
    return res;
}
//============================================================
int NodesDump(Node* tree, FILE* file) {
    if(tree == NULL)
        return 0;
    if(file == NULL)
        return -1;
    NodesDump(tree->left, file);
    fprintf(file, "%d ", tree->data);
    NodesDump(tree->right, file);
    return 0;
}
//============================================================
void* Malloc(size_t size) {
    if(MALLOC_CRUSH_TEST) {
        errno = ENOMEM;
        return NULL;
    }
    return malloc(size);
}
