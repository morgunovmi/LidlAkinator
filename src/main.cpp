#include <stdio.h>

#include "tree.h"
#include "akinator.h"

void PrintElem(void *str) {
    printf("%s\n", (const char *)str);
}

int main() {
    Tree tree = {};

    TreeCtor(&tree, "Nuffin");

    while (true) {
        AkinatorGuess(&tree);
    }

    TreeDtor(&tree);
}
