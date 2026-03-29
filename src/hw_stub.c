#include "hw.h"
#include <stdio.h>

int hw_init(void) {
    fprintf(stderr, "[hw_stub] init\n");
    return 0;
}

void hw_close(void) {
    fprintf(stderr, "[hw_stub] close\n");
}

int hw_pca9685_init(uint8_t address) {
    fprintf(stderr, "[hw_stub] pca9685 init addr=0x%02x\n", address);
    return 0;
}

int hw_pca9685_set_pwm_freq(float hz) {
    fprintf(stderr, "[hw_stub] pwm freq=%.2fHz\n", hz);
    return 0;
}

int hw_pca9685_set_pwm(int channel, int on, int off) {
    fprintf(stderr, "[hw_stub] pwm ch=%d on=%d off=%d\n", channel, on, off);
    return 0;
}

int hw_adc_read(int channel, float *value_out) {
    static const float fake_vals[] = {3.10f, 3.05f, 3.85f};
    if (!value_out || channel < 0 || channel > 2) return -1;
    *value_out = fake_vals[channel];
    return 0;
}

int hw_ultrasonic_get_distance(float *distance_cm_out) {
    if (!distance_cm_out) return -1;
    *distance_cm_out = 42.0f;
    return 0;
}

int hw_infrared_read_all(int *mask_out) {
    if (!mask_out) return -1;
    *mask_out = 2;
    return 0;
}

int hw_buzzer_set(int enabled) {
    fprintf(stderr, "[hw_stub] buzzer=%d\n", enabled);
    return 0;
}
