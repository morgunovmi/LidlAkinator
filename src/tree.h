#ifndef TREE_H 
#define TREE_H 

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/stack.h"
#include "../include/text.h"

#ifndef __FUNCTION_NAME__
    #ifdef WIN32   //WINDOWS
        #define __FUNCTION_NAME__   __FUNCTION__  
    #else          //*NIX
        #define __FUNCTION_NAME__   __func__ 
    #endif
#endif

#define PRINT_ERROR(...) {                                                                                  \
    fprintf(stderr, __VA_ARGS__);                                                                           \
    fprintf(stderr, "file \"%s\", line %d, function \"%s()\"\n", __FILE__, __LINE__, __FUNCTION_NAME__);    \
}                                                                                                           \

const size_t SIZE_POISON = SIZE_MAX;

struct Tree {
    node_t *root;
    size_t size;

    const char *ctorCallFuncName; /**< Function that called the stack constructor */
    const char *ctorCallFile; /**< File of the constructor call */
    int ctorCallLine; /**< Line of the constructor call */

    size_t dumpNum;
};

//! Encapsulates info about function call for debug
struct treeCallInfo {
    const char *funcName; /**< Name of the calling function */
    const char *file; /**< File where the function call happened */
    int line; /**< Line of the function call in the file */
};

enum TreeError : int {
    ERR_NOMEM = 1,
    ERR_DOT_FILE_OPN = 2,
    ERR_FILE_CLS = 3,
};

node_t *ReadSubtree(text_t *text, size_t *curLine);

Tree *ReadTreeFromText(text_t *text);

node_t *CreateNode(const char *val, bool isDynamic);

int TreeCtor_(Tree *tree, const char *val, bool isDynamic, treeCallInfo info);

void FreeSubtree(node_t *node);

void TreeDtor(Tree *tree);

void TreeApplyPrefix(node_t *node, void (*applyf)(void *));

void TreeDumpAux(node_t *node, FILE *file);

int TreeDump_(Tree *tree, const char *reason, treeCallInfo info);

int TreeFind(node_t *node, const char *value, Stack *stack);

#define TreeCtor(tree, val, isDynamic)    \
do {                                           \
    treeCallInfo inf = {};                         \
    inf.funcName = __FUNCTION_NAME__;          \
    inf.file = __FILE__;                       \
    inf.line = __LINE__;                       \
    TreeCtor_(tree, val, isDynamic, inf); \
} while (0)

#define TreeDump(tree, reason)      \
do {                                           \
    treeCallInfo inf = {};                         \
    inf.funcName = __FUNCTION_NAME__;          \
    inf.file = __FILE__;                       \
    inf.line = __LINE__;                       \
    TreeDump_(tree, reason, inf);   \
} while (0)

#endif
