#include "message.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int word_to_int(const char *s) {
    if (strcmp(s, "one") == 0) return 0;
    if (strcmp(s, "two") == 0) return 1;
    if (strcmp(s, "three") == 0) return 3;
    if (strcmp(s, "four") == 0) return 2;
    return 0;
}

bool parse_message(const char *input, parsed_message_t *out) {
    if (!input || !out) return false;
    memset(out, 0, sizeof(*out));
    snprintf(out->raw, sizeof(out->raw), "%s", input);

    char buf[MSG_MAX_RAW];
    snprintf(buf, sizeof(buf), "%s", input);

    char *tok = strtok(buf, "#\r\n");
    if (!tok) return false;

    out->type = command_type_from_string(tok);
    if (out->type == CMD_UNKNOWN) return false;

    while ((tok = strtok(NULL, "#\r\n")) != NULL) {
        if (*tok == '\0') continue;
        if (out->param_count >= MSG_MAX_PARAMS) break;

        char *end = NULL;
        double v = strtod(tok, &end);
        if (end && *end == '\0') {
            out->params[out->param_count++] = (int)(v >= 0 ? v + 0.5 : v - 0.5);
        } else {
            out->params[out->param_count++] = word_to_int(tok);
        }
    }
    return true;
}
