#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include "tree.h"
#include "test.h"

#define ADD(val)                                                            \
    do {                                                                    \
        NewNode = AddNode(tree, val);                                       \
        if(!(NewNode && NewNode->data == val)) {                            \
            TreeDtor(tree);                                                 \
            printf("\nTestAddNode failured on value = %d\n", val);          \
            return -1;                                                      \
        }                                                                   \
    } while(0)

#define VISITOR(order)                                                      \
    do {                                                                    \
        max = 0;                                                            \
        Visitor(tree, order, MaxOdd, (void*)(&max));                        \
        if(max != 99) {                                                     \
            TreeDtor(tree);                                                 \
            printf("\nTestVisitor failured\n");                             \
            return -1;                                                      \
        }                                                                   \
    } while(0)

#define TEST(FuncName)                                                      \
    do {                                                                    \
        if(Test##FuncName())                                                \
            return -1;                                                      \
    } while(0)

//============================================================
//============================================================
int main() {
    if(!unit_test())
        printf("\nAll tests are ok\n");
    return 0;
}
//============================================================
//============================================================
int unit_test(void) {
    TEST(TreeCtor);
    TEST(AddNode);
    TEST(FindElement);
    TEST(Visitor);
    TEST(TreeDump);
    TEST(Malloc);
    return 0;
}
//============================================================
int TestTreeCtor(void) {
    int TestValue = 15;
    Node* tree = TreeCtor(TestValue);
    if(tree == NULL) {
        printf("\nTestTreeCtor: malloc error\n");
        return -1;
    }
    if(tree->data != TestValue || tree->left != NULL || tree->right != NULL) {
        TreeDtor(tree);
        printf("\nTestTreeCtor failured\n");
        return -1;
    }
    TreeDtor(tree);
    return 0;
}
//============================================================
int TestAddNode(void) {
    int TestArray[] = {45, 5, 15, 100, 90, 5, 150, -7, 0, 99};
    int NodesQt = 10;
    Node* NewNode = NULL;
    Node* tree = TreeCtor(TestArray[0]);
    for(int i = 1; i < NodesQt; i++) {
        ADD(TestArray[i]);
    }
    TreeDtor(tree);
    return 0;
}
//============================================================
int TestFindElement(void) {
    int TestArray[] = {100, 200, -50, -100, 49, 52, 47};
    int NodesQt = 7;
    Node* tree = TreeCtor(TestArray[0]);
    for(int i = 1; i < NodesQt; i++)
        AddNode(tree, TestArray[i]);
    for(int i = 0; i < NodesQt; i++) {
        if(!FindElement(tree, TestArray[i])) {
            printf("\nTestFindElement failured\n");
            return -1;
        }
    }
    if(FindElement(tree, 700)) {
            printf("\nTestFindElement failured\n");
            return -1;
    }
    TreeDtor(tree);
    return 0;
}
//============================================================
void MaxOdd(Node* node, void* max) {
    int* max_  = (int*)max;
    if(node->data % 2 && node->data > *max_)
        *max_ = node->data;
}
//============================================================
int TestVisitor(void) {
    int TestArray[] = {45, 5, 15, 100, 90, 77, 150, -7, 0, 99};
    int NodesQt = 10;
    Node* tree = TreeCtor(TestArray[0]);
    for(int i = 1; i < NodesQt; i++)
        AddNode(tree, TestArray[i]);
    int max = 0;
    VISITOR(PRE_ORDER);
    VISITOR(IN_ORDER);
    VISITOR(POST_ORDER);

    if(Visitor(tree, 18, NULL, NULL) != EINVAL) {
        TreeDtor(tree);
        printf("\nTestVisitor failured on wrong order test\n");
        return -1;
    }

    TreeDtor(tree);

    if(Visitor(NULL, 0, NULL, NULL) != EFAULT) {
        printf("\nTestVisitor failured on NULL ptr test\n");
        return -1;
    }

    return 0;
}
//============================================================
int TestTreeDump(void) {
    int TestArray[] = {45, 5, 15, 100, 90, 77, 150, -7, 0, 99};
    int NodesQt = 10;
    Node* tree = TreeCtor(TestArray[0]);
    for(int i = 1; i < NodesQt; i++)
        AddNode(tree, TestArray[i]);
    TreeDump(tree, "dump");
    FILE* dump_file = fopen("dump", "r");
    if(dump_file == NULL) {
        perror("fopen");
        printf("\nCheckPrint failured on fopen\n");
        TreeDtor(tree);
        return -1;
    }
    int CurData = 0;
    int CheckArray[] = {-7, 0, 5, 15, 45, 77, 90, 99, 100, 150};
    for(int i = 0; i < NodesQt; i++) {
        fscanf(dump_file, "%d ", &CurData);
        if(CurData != CheckArray[i]) {
            TreeDtor(tree);
            printf("\nCheckPrint failured\n");
            return -1;
        }
    }
    fclose(dump_file);
    if(TreeDump(tree, "") != -1) {
        TreeDtor(tree);
        printf("\nTestTreeDump failured on wrong pathname test\n");
        return -1;
    }
    if(NodesDump(tree, NULL) != -1) {
        TreeDtor(tree);
        printf("\nTestTreeDump failured on bad file test\n");
        return -1;
    }
    TreeDtor(tree);
    if(TreeDump(NULL, "dump") != EFAULT) {
        printf("\nTestTreeDump failured on NULL tree ptr test\n");
        return -1;
    }
    return 0;
}
//============================================================
int TestMalloc(void) {
    MALLOC_CRUSH_TEST = 1;
    Node* tree = TreeCtor(15);
    if(tree == NULL && errno == ENOMEM)
        return 0;
    printf("\nTestMalloc failured\n");
    return -1;
}


#undef ADD
#undef VISITOR
