#include "motor.h"
#include "hw.h"
#include "util.h"

enum {
    CH_LU_A = 0,
    CH_LU_B = 1,
    CH_LL_A = 3,
    CH_LL_B = 2,
    CH_RU_A = 6,
    CH_RU_B = 7,
    CH_RL_A = 4,
    CH_RL_B = 5,
};

static void set_wheel(int ch_a, int ch_b, int duty) {
    duty = clamp_int(duty, -4095, 4095);
    if (duty > 0) {
        hw_pca9685_set_pwm(ch_a, 0, 0);
        hw_pca9685_set_pwm(ch_b, 0, duty);
    } else if (duty < 0) {
        hw_pca9685_set_pwm(ch_b, 0, 0);
        hw_pca9685_set_pwm(ch_a, 0, -duty);
    } else {
        hw_pca9685_set_pwm(ch_a, 0, 4095);
        hw_pca9685_set_pwm(ch_b, 0, 4095);
    }
}

int motor_init(void) {
    hw_pca9685_init(0x40);
    return hw_pca9685_set_pwm_freq(50.0f);
}

void motor_set_model(int duty1, int duty2, int duty3, int duty4) {
    set_wheel(CH_LU_A, CH_LU_B, duty1);
    set_wheel(CH_LL_A, CH_LL_B, duty2);
    set_wheel(CH_RU_A, CH_RU_B, duty3);
    set_wheel(CH_RL_A, CH_RL_B, duty4);
}

void motor_close(void) {
    motor_set_model(0, 0, 0, 0);
}
