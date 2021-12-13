#include <stdio.h>
#include <string.h>

#include "tree.h"
#include "akinator.h"
#include "../include/text.h"

extern bool ifSay;
//TODO ifSay other way
//TODO different tree dump modes through cl flags
//TODO --help
//TODO Exit dump mode by key

int main(int argc, char **argv) {
    Tree *tree = nullptr;
    text_t text = {};

    if (argc > 3) {
        fprintf(stderr, "Too many arguments\n");

    } else if (argc == 3) {
        if (strcmp(argv[1], "--say") == 0) {
            ifSay = true;

        } else {
            fprintf(stderr, "Undefined flag %s\n", argv[1]);
        }

        if (getText(argv[2], &text) != 1) {
            return EXIT_FAILURE;
        }

        tree = ReadTreeFromText(&text);
        if (!tree) {
            freeText(&text);
            return EXIT_FAILURE;
        }
        
    } else if (argc == 2) {
        if (strcmp(argv[1], "--say") == 0) {
            ifSay = true;

            tree = (Tree *)calloc(1, sizeof(Tree));

            TreeCtor(tree, "nuffin", false);

        } else {
            if (getText(argv[1], &text) != 1) {
                return EXIT_FAILURE;
            }

            tree = ReadTreeFromText(&text);
            if (!tree) {
                freeText(&text);
                return EXIT_FAILURE;
            }
        }

    } else {
        tree = (Tree *)calloc(1, sizeof(Tree));

        TreeCtor(tree, "nuffin", false);
    }

    if (AkinatorPlay(tree) != 0) {
        freeText(&text);
        TreeDtor(tree);
        return EXIT_FAILURE;
    }

    freeText(&text);
    TreeDtor(tree);
}
