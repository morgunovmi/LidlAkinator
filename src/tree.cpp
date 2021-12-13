#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdint.h>

#include "tree.h"

node_t *ReadSubtree(text_t *text, size_t *curLine) {
    assert(text);
    assert(curLine);

    if (*curLine + 1 >= text->numLines) {
        return nullptr;

    } else if (strcmp(text->lines[(*curLine)++].ptr, "{") != 0) {
        return nullptr;
    }

    node_t *node = CreateNode(text->lines[(*curLine)++].ptr, false);

    if (strcmp(text->lines[*curLine].ptr, "{") == 0) {
        node->left = ReadSubtree(text, curLine);
    }

    if (strcmp(text->lines[*curLine].ptr, "{") == 0) {
        node->right = ReadSubtree(text, curLine);
    }
    
    if (*curLine + 1 >= text->numLines) {
        return nullptr;

    } else if (strcmp(text->lines[(*curLine)++].ptr, "}") != 0) {
        return nullptr;
    }

    return node;
}

Tree *ReadTreeFromText(text_t *text) {
    assert(text);

    Tree *tree = (Tree *)calloc(1, sizeof(Tree));
    if (!tree) {
        PRINT_ERROR("Error allocating memory for the tree : %s\n", strerror(errno));
        return nullptr;
    }

    size_t curLine = 0;
    if (strcmp(text->lines[curLine++].ptr, "{") != 0) {
        PRINT_ERROR("Expected opening \"{\"");
        return nullptr;
    }
    
    TreeCtor(tree, text->lines[curLine++].ptr, false); 

    tree->root->left = ReadSubtree(text, &curLine);
    tree->root->right = ReadSubtree(text, &curLine);

    if (strcmp(text->lines[curLine++].ptr, "}") != 0) {
        PRINT_ERROR("Expected terminating \"}\"");
        return nullptr;
    }

    return tree;
}

node_t *CreateNode(const char *val, bool isDynamic) {
    assert(val);

    node_t *node = (node_t *)calloc(1, sizeof(*node));
    
    if (!node) {
        PRINT_ERROR("Error allocating memory for node : %s\n", strerror(errno));
        return nullptr;
    }

    if (!isDynamic) {
        node->data = (char *)val;
    } else {
        node->data = strdup(val); 
    }

    if (!node->data) {
        PRINT_ERROR("Error allocating memory for string : %s\n", strerror(errno));
        return nullptr;
    }

    node->left = nullptr;
    node->right = nullptr;
    node->isDynamic = isDynamic;
    return node;
}

int TreeCtor_(Tree *tree, const char *val, bool isDynamic, treeCallInfo info) {
    assert(tree);
    assert(tree->root == nullptr);
    assert(tree->size == 0);

    system("rm -rf dumps");

    tree->root = CreateNode(val, isDynamic);

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

    if (node->isDynamic) {
        free(node->data);
    }
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

    free(tree);
}

void TreeDumpAux(node_t *node, FILE *file) {
    fprintf(file, "    \"%s\" [label=\" { %s | {  %p | %p }} \"];",
            node->data, node->data,
            node->left, node->right); 

    if (node->left) {
        fprintf(file, "    \"%s\" [label=\" { %s | {  %p | %p }} \"];",
                node->left->data, node->left->data,
                node->left->left, node->left->right); 

        fprintf(file, "    \"%s\" -> \"%s\"[color=\"green\", label=\"yes\"];\n",
                node->data, node->left->data);
        TreeDumpAux(node->left, file);
    }

    if (node->right) {
        fprintf(file, "    \"%s\" [label=\" { %s | {  %p | %p }} \"];",
                node->right->data, node->right->data,
                node->right->left, node->right->right); 

        fprintf(file, "    \"%s\" -> \"%s\"[color=\"red\", label=\"no\"];\n",
                node->data, node->right->data);
        TreeDumpAux(node->right, file);
    }
}

int TreeDump_(Tree *tree, const char *reason, treeCallInfo info) {
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
            "| { %p | %zu }\"];\n\n", (void *)tree->root, tree->size);

    fprintf(dotFile, "    node [shape=record, fontname=\"Arial\"];\n");

    if (!tree->root->left && !tree->root->right) {
        fprintf(dotFile, "    \"%s\" [label=\" { %s | {  %p | %p }} \"];",
                tree->root->data, tree->root->data,
                tree->root->left, tree->root->right); 

    } else {
        TreeDumpAux(tree->root, dotFile);
    }

    fprintf(dotFile, "\n}\n");

    if (fclose(dotFile) == -1) {
        PRINT_ERROR("Error closing dot file : %s\n", strerror(errno));
        return ERR_FILE_CLS;
    }

    char command[128] = "";
    sprintf(command, "dot -Tjpg %s -o dumps/dump%zu.jpg", dotFileName, tree->dumpNum);
    system(command);
    sprintf(command, "rm %s\n", dotFileName);
    system(command);
    sprintf(command, "fim dumps/dump%zu.jpg", tree->dumpNum);
    system(command);

    tree->dumpNum++;

    return 0;
}

int TreeFind(node_t *node, const char *value, Stack *stack) {
    if (!node->left && !node->right) {
        if (strcmp(node->data, value) == 0) {
            return 0;

        } else {
            return 1;
        } 
    }

    StackPush(stack, node);

    int res = 0;

    if (TreeFind(node->left, value, stack) == 0) {
        return 0;
    } else {
        res = 1;
    }

    if (TreeFind(node->right, value, stack) == 0) {
        return 0;
    } else {
        res = 1;
    }

    if (res == 1) {
        node_t temp = {};
        StackPop(stack, &temp); 
    }

    return res;
}
