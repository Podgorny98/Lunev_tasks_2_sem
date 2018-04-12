enum TRAVERSAL_ORDER {
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER
};

struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
};
typedef struct TreeNode Node;
int MALLOC_CRUSH_TEST;

Node* TreeCtor(int val);
Node* AddNode(Node* tree, int val);
void TreeDtor(Node* tree);
int FindElement(Node* tree, int val);
int Visitor(Node* tree, enum TRAVERSAL_ORDER order, void (*func)(Node* node, void* ctx), void* ctx);
int TreeDump(Node* tree, const char* pathname);
int NodesDump(Node* tree, FILE* file);
void* Malloc(size_t size);
