#include <stdio.h>

#include "tree.h"

void PrintElem(void *str) {
    printf("%s\n", (const char *)str);
}

int main() {
    Tree tree = {};

    TreeCtor(&tree, "Name jeff");

    tree.root->left = CreateNode("Hello jeff");
    tree.root->right = CreateNode("Hi");

    TreeApplyPrefix(tree.root, PrintElem);

    TreeDtor(&tree);
}
