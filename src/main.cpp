#include <stdio.h>
#include <string.h>
#include <argp.h>

#include "tree.h"
#include "akinator.h"
#include "../include/text.h"

extern bool ifSay;
extern bool ifDebug;

//TODO ifSay other way
//TODO different tree dump modes through cl flags
//TODO Exit dump mode by key

const char *argp_program_bug_address = "<maxexmachina@gmail.com>";

static char doc[] = "A lidl akinator for you command line.";

static struct argp_option options[] = {
    {"say", 's', 0, 0, "Use text to speech"},
    {"debug", 'd', 0, 0, "Makes tree dump debug friendly"},
    {"file", 'f', "FILE_PATH", 0, "Use a custom file for the database"}
};

struct arguments {
    int say, debug;
    char *dbFilePath;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = (struct arguments *)state->input;

    switch (key) {
        case 's':
            arguments->say = 1;
            break;
        case 'd':
            arguments->debug = 1;
            break;
        case 'f':
            arguments->dbFilePath = arg;
            break;
        case ARGP_KEY_ARG:
            argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, 0, doc };

int main(int argc, char **argv) {
    struct arguments arguments = {};

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (arguments.say) {
        ifSay = true;
    }

    if (arguments.debug) {
        ifDebug = true;
    }

    Tree *tree = nullptr;
    text_t text = {};

    if (arguments.dbFilePath) {
        if (getText(arguments.dbFilePath, &text) != 1) {
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
