#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <argp.h> 
#include <assert.h>

const char *argp_program_bug_address = "<maxexmachina@gmail.com>";
static char doc[] = "A lidl akinator for you command line.";

static struct argp_option options[] = {
    {"say", 's', 0, 0, "Use text to speech"},
    {"debug", 'd', 0, 0, "Makes tree dump debug friendly"},
    {"file", 'f', "FILE_PATH", 0, "Use a custom file for the database"}
};

struct arguments {
    bool say, debug;
    char *dbFilePath;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    assert(state);

    struct arguments *arguments = (struct arguments *)state->input;

    switch (key) {
        case 's':
            arguments->say = true; 
            break;
        case 'd':
            arguments->debug = true;
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

#endif
