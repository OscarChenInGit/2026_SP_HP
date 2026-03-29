#include "command.h"
#include <string.h>

typedef struct {
    const char *name;
    command_type_t type;
} command_map_t;

static const command_map_t k_map[] = {
    {"CMD_MOTOR", CMD_MOTOR},
    {"CMD_M_MOTOR", CMD_M_MOTOR},
    {"CMD_CAR_ROTATE", CMD_CAR_ROTATE},
    {"CMD_LED", CMD_LED},
    {"CMD_LED_MOD", CMD_LED_MOD},
    {"CMD_SERVO", CMD_SERVO},
    {"CMD_BUZZER", CMD_BUZZER},
    {"CMD_SONIC", CMD_SONIC},
    {"CMD_LIGHT", CMD_LIGHT},
    {"CMD_POWER", CMD_POWER},
    {"CMD_MODE", CMD_MODE},
    {"CMD_LINE", CMD_LINE},
};

const char *command_type_name(command_type_t type) {
    for (size_t i = 0; i < sizeof(k_map) / sizeof(k_map[0]); ++i) {
        if (k_map[i].type == type) return k_map[i].name;
    }
    return "CMD_UNKNOWN";
}

command_type_t command_type_from_string(const char *s) {
    if (!s) return CMD_UNKNOWN;
    for (size_t i = 0; i < sizeof(k_map) / sizeof(k_map[0]); ++i) {
        if (strcmp(k_map[i].name, s) == 0) return k_map[i].type;
    }
    return CMD_UNKNOWN;
}
