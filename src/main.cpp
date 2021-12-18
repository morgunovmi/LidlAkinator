#include <stdio.h>
#include <string.h>
#include <argp.h>

#include "tree.h"
#include "akinator.h"
#include "../include/text.h"
#include "argParser.h"

extern bool ifSay;
extern bool ifDebug;

//TODO ifSay other way

int main(int argc, char **argv) {
    char *dbFilePath = "";

    parseArgs(argc, argv, &ifSay, &ifDebug, &dbFilePath);

    Tree *tree = nullptr;
    text_t text = {};

    if (dbFilePath) {
        if (getText(dbFilePath, &text) != 1) {
            return EXIT_FAILURE;
        }

        tree = ReadTreeFromText(&text);
        if (!tree) {
            freeText(&text);
            return EXIT_FAILURE;
        }

    } else {
        tree = (Tree *)calloc(1, sizeof(Tree));
        if (!tree) {
            return EXIT_FAILURE;
        }

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
