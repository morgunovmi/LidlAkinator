#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

#include "tree.h"

node_t *CreateNode(const char *val) {
    assert(val);

    node_t *node = (node_t *)calloc(1, sizeof(*node));
    
    if (!node) {
        PRINT_ERROR("Error allocating memory for node : %s\n", strerror(errno));
        return nullptr;
    }

    const char *str = strdup(val); 

    if (!str) {
        PRINT_ERROR("Error allocating memory for string : %s\n", strerror(errno));
        return nullptr;
    }

    node->data = (char *)str;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

int TreeCtor_(Tree *tree, const char *val, callInfo info) {
    assert(tree);
    assert(tree->root == nullptr);
    assert(tree->size == 0);

    tree->root = CreateNode(val);

    if (!tree->root) {
        return ERR_NOMEM;
    }

    tree->size = 1;

    tree->ctorCallFuncName = info.funcName;
    tree->ctorCallFile = info.file;
    tree->ctorCallLine = info.line; 

    return 0;
}

void FreeSubtree(node_t *node) {
    if (!node) {
        return;
    }

    FreeSubtree(node->left);
    FreeSubtree(node->right);
    free(node->data);
    free(node);
}

void TreeDtor(Tree *tree) {
    assert(tree);
    assert(tree->root);

    FreeSubtree(tree->root);
    tree->root = (node_t *)13;
    tree->size = SIZE_POISON;
}

void TreeApplyPrefix(node_t *node, void (*applyf)(void *)) {
    if (!node) {
        return;
    }

    applyf(node->data);
    TreeApplyPrefix(node->left, applyf);
    TreeApplyPrefix(node->right, applyf);
}
