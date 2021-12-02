#include <stdio.h>

#include "tree.h"
#include "akinator.h"

int main() {
    Tree tree = {};

    TreeCtor(&tree, "nuffin");

    if (AkinatorPlay(&tree) != 0) {
        return EXIT_FAILURE;
    }

    TreeDtor(&tree);
}
