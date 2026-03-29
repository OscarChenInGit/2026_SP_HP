#include "car.h"
#include "hw.h"
#include "message.h"
#include "motor.h"
#include "servo.h"
#include "util.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static void handle_message(car_t *car, const parsed_message_t *msg) {
    switch (msg->type) {
        case CMD_BUZZER:
            if (msg->param_count >= 1) hw_buzzer_set(msg->params[0]);
            break;
        case CMD_SERVO:
            if (msg->param_count >= 2) servo_set_angle(msg->params[0], msg->params[1]);
            break;
        case CMD_MOTOR:
            if (msg->param_count >= 4) {
                const float scale = 0.8f;
                motor_set_model(
                    (int)lround(msg->params[0] * scale),
                    (int)lround(msg->params[1] * scale),
                    (int)lround(msg->params[2] * scale),
                    (int)lround(msg->params[3] * scale)
                );
            }
            break;
        case CMD_M_MOTOR:
            if (msg->param_count >= 4) {
                int a1 = msg->params[0], m1 = msg->params[1], a2 = msg->params[2], m2 = msg->params[3];
                int lx = -(int)(m1 * sin(a1 * 3.14159265358979323846 / 180.0));
                int ly =  (int)(m1 * cos(a1 * 3.14159265358979323846 / 180.0));
                int rx =  (int)(m2 * sin(a2 * 3.14159265358979323846 / 180.0));
                int fr = ly - lx + rx;
                int fl = ly + lx - rx;
                int bl = ly - lx - rx;
                int br = ly + lx + rx;
                motor_set_model(fl, bl, fr, br);
            }
            break;
        case CMD_SONIC: {
            float distance = 0.0f;
            hw_ultrasonic_get_distance(&distance);
            printf("CMD_MODE#3#%.2f\n", distance);
            break;
        }
        case CMD_LIGHT: {
            float l = 0.0f, r = 0.0f;
            hw_adc_read(0, &l); hw_adc_read(1, &r);
            printf("CMD_MODE#2#%.2f#%.2f\n", l, r);
            break;
        }
        case CMD_LINE: {
            int v = 0;
            hw_infrared_read_all(&v);
            printf("CMD_MODE#4#%d#%d#%d\n", !!(v & 0x1), !!(v & 0x2), !!(v & 0x4));
            break;
        }
        case CMD_POWER: {
            float p = 0.0f;
            hw_adc_read(2, &p);
            printf("CMD_POWER#%.2f\n", p * 2.0f);
            break;
        }
        default:
            printf("Unhandled: %s\n", command_type_name(msg->type));
            break;
    }
}

int main(void) {
    if (hw_init() != 0) {
        fprintf(stderr, "hardware init failed\n");
        return 1;
    }

    car_t car;
    if (car_init(&car) != 0) {
        fprintf(stderr, "car init failed\n");
        hw_close();
        return 1;
    }

    puts("Freenove C refactor demo");
    puts("Enter commands like:");
    puts("  CMD_MOTOR#1000#1000#1000#1000");
    puts("  CMD_SERVO#0#90");
    puts("  CMD_SONIC");

    char line[256];
    while (fgets(line, sizeof(line), stdin)) {
        parsed_message_t msg;
        if (!parse_message(line, &msg)) {
            fprintf(stderr, "Invalid message: %s", line);
            continue;
        }
        handle_message(&car, &msg);
    }

    car_close(&car);
    hw_close();
    return 0;
}
