#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "akinator.h"

uint16_t AkinatorModeSelect() {
    uint16_t mode = 0;

    printf("Please choose the mode\n");
    while (true) {
        printf("1: Guess the object\n");

        if (scanf("%hu", &mode) != 1 ||
            mode == 0 || mode > NUM_MODES) {
            printf("Please enter a valid mode\n");
            while (getc(stdin) != '\n')
                ;
            continue;
        }

        while (getc(stdin) != '\n')
            ;
        break;
    }

    return mode;
}

int GetAnswer() {
    char scanned;
    while (1) {
        scanf("%c", &scanned);
        scanned = tolower(scanned);

        if (scanned == 'y' || scanned == 'n') {
            while (getc(stdin) != '\n')
                ;

            break;
        }

        while (getc(stdin) != '\n')
            ;
        printf("Please type y or n!\n");
    }

    switch (scanned) {
        case 'y':
            return 1;
        case 'n':
            return 0;
        default:
            return -1;
    }
}

int AkinatorAddItem(node_t *node) {
    printf("Can you tell me what you guessed?\n");

    char itemToAdd[256] = "";
    while (true) {
        if (scanf("%[^\n]s", itemToAdd) != 1) {
            printf("Please enter a valid name\n");
            continue;
        }
        while (getc(stdin) != '\n')
            ;
        break;
    }

    printf("Now tell me, what's %s that %s is not?\n", itemToAdd, node->data);

    char diffTrait[256] = "";
    while (true) {
        if (scanf("%[^\n]s", diffTrait) != 1) {
            printf("Please enter a valid trait\n");
            continue;
        }
        while (getc(stdin) != '\n')
            ;
        break;
    }

    node->left = CreateNode(itemToAdd);
    node->right = CreateNode(node->data);
    free(node->data);
    node->data = strdup(diffTrait);

    return 0;
}

int AkinatorGuess(Tree *tree) {
    assert(tree);
    assert(tree->root);

    printf("Welcome to my world, meat sack! \nI'm going to effortlessly "
            "guess what you have in mind now! Check me out!\n\n");

    node_t *cur = tree->root; 
    while (!(cur->left == nullptr && cur->right == nullptr)) {
        printf("Is your object %s?\n", cur->data);

        int answer = GetAnswer();
        if (answer == 1) {
            cur = cur->left;

        } else if (answer == 0) {
            cur = cur->right;

        } else {
            PRINT_ERROR("Got an invalid answer\n");
            return INV_ANSWER;
        } 
    }
    printf("Did you guess %s?\n",  cur->data);

    int answer = GetAnswer();
    if (answer == 1) {
        printf("EZ game you fucking meat sack loser!\nResistance is futile!\n"); 

    } else if (answer == 0) {
        if (AkinatorAddItem(cur) != 0) {
            PRINT_ERROR("Error adding item to akinator tree\n");
            return ADD_ITEM_ERR;
        }

    } else {
        PRINT_ERROR("Got an invalid answer\n");
        return INV_ANSWER;
    }

    return 0;
}
