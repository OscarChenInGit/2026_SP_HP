#include "servo.h"
#include "hw.h"

static const int k_pwm_map[8] = {8, 9, 10, 11, 12, 13, 14, 15};

static int angle_to_pulse_us(int logical_channel, int angle_deg, int error_deg) {
    if (logical_channel == 0) {
        return 2500 - (int)((angle_deg + error_deg) / 0.09);
    }
    return 500 + (int)((angle_deg + error_deg) / 0.09);
}

int servo_init(void) {
    hw_pca9685_init(0x40);
    hw_pca9685_set_pwm_freq(50.0f);
    for (int i = 0; i < 8; ++i) {
        int pulse_us = 1500;
        int off = pulse_us * 4096 / 20000;
        hw_pca9685_set_pwm(k_pwm_map[i], 0, off);
    }
    return 0;
}

void servo_set_angle(int logical_channel, int angle_deg) {
    if (logical_channel < 0 || logical_channel > 7) return;
    int pulse_us = angle_to_pulse_us(logical_channel, angle_deg, 10);
    int off = pulse_us * 4096 / 20000;
    hw_pca9685_set_pwm(k_pwm_map[logical_channel], 0, off);
}

void servo_close(void) {
}
