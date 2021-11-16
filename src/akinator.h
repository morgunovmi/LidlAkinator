#ifndef AKINATOR_H
#define AKINATOR_H

#include "tree.h"

enum AkinatorError : int {
    INV_ANSWER = 1,
    ADD_ITEM_ERR = 2,
};

int AkinatorGuess(Tree *tree);

int AkinatorAddItem(node_t *node);

int GetAnswer();

#endif
