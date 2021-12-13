#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>

#include "tree.h"

enum AkinatorError : int {
    INV_ANSWER = 1,
    ADD_ITEM_ERR = 2,
    ERR_OPN_FILE = 3,
    ERR_CLS_FILE = 4,
    SAVE_ERR = 5,
    ERR_SPRINTF = 6,
};

enum AkinatorMode : int {
    MODE_GUESS = 1,
    MODE_DUMP = 2,
    MODE_DEFINE = 3,
    MODE_COMPARE = 4
};

static const char *MODE_DESCRS[] = {
    "Guess the object",
    "Dump the database",
    "Define the object",
    "Compare two objects"
};

const size_t NUM_MODES = sizeof(MODE_DESCRS) / sizeof(*MODE_DESCRS);

#define MAX_STR_LEN 1024

int AkinatorSay(const char *format ...);

void ClearStdin();

int AkinatorScanLine(char *str);

int AkinatorPlay(Tree *tree);

int AkinatorModeSelect();

int AkinatorGuess(Tree *tree);

void AkinatorEnumerateTraits(const char *object, Stack *propertyStack,
        size_t idx);

int AkinatorDefine(Tree *tree);

int AkinatorCompare(Tree *tree);

int AkinatorAddItem(node_t *node);

int GetAnswer();

int SaveSubtree(FILE *file, node_t *node);

int AkinatorSaveDb(Tree *tree);

#endif
