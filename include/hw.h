#ifndef HW_H
#define HW_H

#include <stdint.h>

int hw_init(void);
void hw_close(void);

int hw_pca9685_init(uint8_t address);
int hw_pca9685_set_pwm_freq(float hz);
int hw_pca9685_set_pwm(int channel, int on, int off);

int hw_adc_read(int channel, float *value_out);
int hw_ultrasonic_get_distance(float *distance_cm_out);
int hw_infrared_read_all(int *mask_out);
int hw_buzzer_set(int enabled);

#endif
