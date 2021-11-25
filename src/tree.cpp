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

    node->data = strdup(val); 

    if (!node->data) {
        PRINT_ERROR("Error allocating memory for string : %s\n", strerror(errno));
        return nullptr;
    }

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

    tree->dumpNum = 0;

    return 0;
}

void FreeSubtree(node_t *node) {
    if (!node) {
        return;
    }

    free(node->data);
    FreeSubtree(node->left);
    FreeSubtree(node->right);
    free(node);
}

void TreeDtor(Tree *tree) {
    assert(tree);
    assert(tree->root);

    FreeSubtree(tree->root);
    tree->root = (node_t *)13;
    tree->size = SIZE_POISON;
}

int TreeDump_(Tree *tree, const char *reason, callInfo info) {
    assert(tree);
    assert(tree->root);

    system("mkdir -p dumps\n");

    char dotFileName[64] = "";
    sprintf(dotFileName, "dumps/dump%zu.dot", tree->dumpNum);

    FILE *dotFile = fopen(dotFileName, "w");
    if (!dotFile) {
        PRINT_ERROR("Error opening file for dump : %s\n", strerror(errno));
        return ERR_DOT_FILE_OPN;
    }

    fprintf(dotFile, "digraph tree{\n"
                    "{\nrankdir=HR;\n"
                    "node[shape=plaintext];\nedge[color=white]\n"
                     "\"Tree<%s>[%p]\n dumped from %s() at %s (%d)\n\n", 
                             "const char *", (void *)tree, info.funcName,
							 info.file, info.line);

    fprintf(dotFile, "Constructed in %s() at %s (%d)\n"
                             "Dump reason : %s\"\n}\n",
                             tree->ctorCallFuncName,
							 tree->ctorCallFile, tree->ctorCallLine, reason);

    fprintf(dotFile, "data [shape=record, label=\"{ root | size }"
            "| { %p | %zu }\"];", (void *)tree->root, tree->size);

    fprintf(dotFile, "}\n");

    if (fclose(dotFile) == -1) {
        PRINT_ERROR("Error closing dot file : %s\n", strerror(errno));
        return ERR_FILE_CLS;
    }

    char command[128] = "";
    sprintf(command, "dot -Tjpg %s -o dumps/dump%zu.jpg", dotFileName, tree->dumpNum);
    system(command);
    sprintf(command, "rm %s\n", dotFileName);
    system(command);

    tree->dumpNum++;

    return 0;
}
