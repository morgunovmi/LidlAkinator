#include <stdio.h>

#include "tree.h"
#include "akinator.h"

int main() {
    Tree tree = {};

    TreeCtor(&tree, "nuffin");

    // TODO Extract AkinatorPlay func
    while (true) {
        uint16_t mode = AkinatorModeSelect();

        if (mode == 0) {
            PRINT_ERROR("Invalid mode selected\n");
            TreeDtor(&tree);
            return EXIT_FAILURE;
        }

        switch (mode) {
            case MODE_GUESS:
                AkinatorGuess(&tree);
                break;

            default:
                PRINT_ERROR("Invalid mode specified\n");
                TreeDtor(&tree);
                return EXIT_FAILURE;
        }

        printf("Do you wish to play another round?\n");
        int ans = GetAnswer();
        if (ans == -1) { 
            PRINT_ERROR("Invalid answer returned\n");
            TreeDtor(&tree);
            return EXIT_FAILURE;

        } else if (ans == 0) {
            printf("Do you wish to save the current database?\n");
            ans = GetAnswer();

            if (ans == 1) {
                AkinatorSaveDb(&tree);

            } else if (ans == -1) {
                PRINT_ERROR("Invalid answer returned\n");
                TreeDtor(&tree);
                return EXIT_FAILURE;
            }

            break;
        }
    }

    TreeDtor(&tree);
}
