#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include "akinator.h"
#include "../include/stack.h"

int AkinatorPlay(Tree *tree) {
    assert(tree);

    while (true) {
        uint16_t mode = AkinatorModeSelect();

        if (mode == 0) {
            PRINT_ERROR("Invalid mode selected\n");
            TreeDtor(tree);
            return 1;
        }

        switch (mode) {
            case MODE_GUESS:
                AkinatorGuess(tree);
                break;

            case MODE_DUMP:
                TreeDump(tree, "mode_dump");
                break;

            case MODE_DEFINE:
                AkinatorDefine(tree);
                break;

            default:
                PRINT_ERROR("Invalid mode specified\n");
                TreeDtor(tree);
                return 1;
        }

        printf("Do you wish to play another round? (y or n)\n");
        int ans = GetAnswer();
        if (ans == -1) { 
            PRINT_ERROR("Invalid answer returned\n");
            TreeDtor(tree);
            return 1;

        } else if (ans == 0) {
            printf("Do you wish to save the current database? (y or n)\n");
            ans = GetAnswer();

            if (ans == 1) {
                AkinatorSaveDb(tree);

            } else if (ans == -1) {
                PRINT_ERROR("Invalid answer returned\n");
                TreeDtor(tree);
                return 1;
            }

            break;
        }
    }

    return 0;
}

uint16_t AkinatorModeSelect() {
    uint16_t mode = 0;

    printf("Please choose the mode\n");
    while (true) {
        for (size_t i = 0; i < NUM_MODES; i++) {
            printf("%zu: %s\n", i + 1, MODE_DESCRS[i]);
        }

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
    assert(node);

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

    node->left = CreateNode(itemToAdd, true);

    node->right = CreateNode(node->data, node->isDynamic);

    if (node->isDynamic) {
        free(node->data);
    }
    node->data = strdup(diffTrait);
    node->isDynamic = true;

    return 0;
}

int AkinatorGuess(Tree *tree) {
    assert(tree);
    assert(tree->root);

    printf("Welcome to my world, meat sack! \nI'm going to effortlessly "
            "guess what you have in mind now! Check me out!\n\n");

    node_t *cur = tree->root; 
    while (!(cur->left == nullptr && cur->right == nullptr)) {
        printf("Is your object %s? (y or n)\n", cur->data);

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
    printf("Did you guess %s? (y or n)\n",  cur->data);

    int answer = GetAnswer();
    if (answer == 1) {
        printf("EZ game you fucking meat sack loser!\nResistance is futile!\n\n"); 

    } else if (answer == 0) {
        if (AkinatorAddItem(cur) != 0) {
            PRINT_ERROR("Error adding item to akinator tree\n");
            return ADD_ITEM_ERR;
        }
        tree->size += 2;

    } else {
        PRINT_ERROR("Got an invalid answer\n");
        return INV_ANSWER;
    }

    return 0;
}

int AkinatorDefine(Tree *tree) {
    assert(tree);

    printf("Please give me an object to define\n");

    char objectToDefine[256] = "";
    while (true) { if (scanf("%[^\n]s", objectToDefine) != 1) { 
            printf("Please enter a valid object\n");
            continue;
        }
        while (getc(stdin) != '\n')
            ;
        break;
    }

    Stack propertyStack = {};

    StackCtor(&propertyStack, sizeof(node_t), 10);
    if (TreeFind(tree->root, objectToDefine, &propertyStack) != 0) {
        printf("There is no such element in the database\n");
        StackDtor(&propertyStack);
        return 1;
    }

    printf("%s is ", objectToDefine);
    for (size_t i = 0; i < propertyStack.size - 1; i++) {
        node_t cur = StackAccess_(&propertyStack, i);

        if (strcmp(StackAccess_(&propertyStack, i + 1).data,
                    cur.right->data) == 0) {
            printf("not "); 
        }

        printf("%s, ", cur.data);
    }

    node_t last = StackAccess_(&propertyStack, propertyStack.size - 1);
    if (strcmp(last.data, objectToDefine) == 0) {
        printf("not "); 
    }

    printf("%s.\n\n", last.data);

    StackDtor(&propertyStack);

    return 0;
}

int SaveSubtree(FILE *file, node_t *node) {
    if (!node) {
        return 0;
    }

    fprintf(file, "{\n%s\n", node->data);

    if (SaveSubtree(file, node->left) != 0) {
        return 1;
    }

    if (SaveSubtree(file, node->right) != 0) {
        return 1;
    }

    fprintf(file, "}\n");
    return 0;
}

int AkinatorSaveDb(Tree *tree) {
    assert(tree);
    assert(tree->root);

    const char *dbname = "db/db.txt";
    FILE *db = fopen(dbname, "w");
    
    if (db == nullptr) {
        PRINT_ERROR("Error opening database file %s : %s\n", dbname, strerror(errno)); 
        return ERR_OPN_FILE;
    }

    if (SaveSubtree(db, tree->root) != 0) {
        return SAVE_ERR;
    }

    if (fclose(db) == EOF) {
        PRINT_ERROR("Error closing database file %s : %s\n", dbname, strerror(errno)); 
        return ERR_CLS_FILE;
    }

    return 0;
}
