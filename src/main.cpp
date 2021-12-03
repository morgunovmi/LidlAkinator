#include <stdio.h>

#include "tree.h"
#include "akinator.h"
#include "../include/text.h"

int main() {
    text_t text = {};
    if (getText("db/db.txt", &text) != 1) {
        return EXIT_FAILURE;
    }

    Tree *tree = ReadTreeFromText(&text);
    if (!tree) {
        freeText(&text);
        return EXIT_FAILURE;
    }

    if (AkinatorPlay(tree) != 0) {
        freeText(&text);
        TreeDtor(tree);
        return EXIT_FAILURE;
    }

    freeText(&text);
    TreeDtor(tree);
}
