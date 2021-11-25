#ifndef AKINATOR_H
#define AKINATOR_H

#include "tree.h"

enum AkinatorError : int {
    INV_ANSWER = 1,
    ADD_ITEM_ERR = 2,
};

enum AkinatorMode : int {
    MODE_GUESS = 1,
};

const uint16_t NUM_MODES = 1;

uint16_t AkinatorModeSelect();

int AkinatorGuess(Tree *tree);

int AkinatorAddItem(node_t *node);

int GetAnswer();

#endif
