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
};

enum AkinatorMode : int {
    MODE_GUESS = 1,
    MODE_DUMP = 2,
};

const uint16_t NUM_MODES = 2;

uint16_t AkinatorModeSelect();

int AkinatorGuess(Tree *tree);

int AkinatorAddItem(node_t *node);

int GetAnswer();

int SaveSubtree(FILE *file, node_t *node);

int AkinatorSaveDb(Tree *tree);

#endif
