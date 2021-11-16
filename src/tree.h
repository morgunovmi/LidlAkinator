#ifndef TREE_H 
#define TREE_H 

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef __FUNCTION_NAME__
    #ifdef WIN32   //WINDOWS
        #define __FUNCTION_NAME__   __FUNCTION__  
    #else          //*NIX
        #define __FUNCTION_NAME__   __func__ 
    #endif
#endif

#define PRINT_ERROR(...) {                                                                                  \
    fprintf(stderr, __VA_ARGS__);                                                                           \
    fprintf(stderr, "file \"%s\", line %d, function \"%s()\"\n", __FILE__, __LINE__, __FUNCTION_NAME__);  \
}                                                                                                           \

const size_t SIZE_POISON = SIZE_MAX;

struct node_t {
    char *data;
    node_t *left;
    node_t *right;
};

struct Tree {
    node_t *root;
    size_t size;

    const char *ctorCallFuncName; /**< Function that called the stack constructor */
    const char *ctorCallFile; /**< File of the constructor call */
    int ctorCallLine; /**< Line of the constructor call */
};

//! Encapsulates info about function call for debug
struct callInfo {
    const char *funcName; /**< Name of the calling function */
    const char *file; /**< File where the function call happened */
    int line; /**< Line of the function call in the file */
};

enum TreeError : int {
    ERR_NOMEM = 1,
};

node_t *CreateNode(const char *val);

int TreeCtor_(Tree *tree, const char *val, callInfo info);

void FreeSubtree(node_t *node);

void TreeDtor(Tree *tree);

void TreeApplyPrefix(node_t *node, void (*applyf)(void *));

#define TreeCtor(tree, val)    \
do {                                           \
    callInfo inf = {};                         \
    inf.funcName = __FUNCTION_NAME__;          \
    inf.file = __FILE__;                       \
    inf.line = __LINE__;                       \
    TreeCtor_(tree, val, inf); \
} while (0)

#endif
