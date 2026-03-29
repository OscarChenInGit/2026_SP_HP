#ifndef MESSAGE_H
#define MESSAGE_H

#include <stddef.h>
#include <stdbool.h>
#include "command.h"

#define MSG_MAX_PARAMS 16
#define MSG_MAX_RAW 256

typedef struct {
    char raw[MSG_MAX_RAW];
    command_type_t type;
    int params[MSG_MAX_PARAMS];
    size_t param_count;
} parsed_message_t;

bool parse_message(const char *input, parsed_message_t *out);

#endif
