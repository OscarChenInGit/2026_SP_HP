#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
    CMD_UNKNOWN = 0,
    CMD_MOTOR,
    CMD_M_MOTOR,
    CMD_CAR_ROTATE,
    CMD_LED,
    CMD_LED_MOD,
    CMD_SERVO,
    CMD_BUZZER,
    CMD_SONIC,
    CMD_LIGHT,
    CMD_POWER,
    CMD_MODE,
    CMD_LINE
} command_type_t;

const char *command_type_name(command_type_t type);
command_type_t command_type_from_string(const char *s);

#endif
