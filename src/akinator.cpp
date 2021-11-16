#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>

#include "akinator.h"

int GetAnswer() {
    int answer = -1;
    printf("Getting answer\n");

    char scanned = 0;
    while ((scanned = getc(stdin)) != EOF && scanned != '\n') {
        if (scanned == 'y') {
            return 1;

        } else if (scanned == 'n') {
            return 0;
        }
    } 

    printf("Got answer\n");
    return answer;
}

int AkinatorAddItem(node_t *node) {
    printf("Can you tell me what you guessed?\n");

    char itemToAdd[256] = "";
    while (true) {
        if (scanf("%[^\n]s", itemToAdd) != 1) {
            printf("Please enter a valid name\n");
            continue;
        }
        break;
    }

    printf("Now tell me, what's %s that %s is not?\n", itemToAdd, node->data);

    char diffTrait[256] = "";
    while (true) {
        if (scanf("%[^\n]s", itemToAdd) != 1) {
            printf("Please enter a valid trait\n");
            continue;
        }
        break;
    }

    node->left = CreateNode(itemToAdd);
    node->right = CreateNode(node->data);
    node->data = diffTrait;

    return 0;
}

int AkinatorGuess(Tree *tree) {
    assert(tree);
    assert(tree->root);

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
