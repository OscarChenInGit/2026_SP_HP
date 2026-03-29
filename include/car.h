#ifndef CAR_H
#define CAR_H

#include <stdbool.h>

typedef struct {
    int sonic_servo_angle;
    int sonic_servo_dir;
    float sonic_distance[3];
    double last_tick_s;
    int time_compensate;
} car_t;

int car_init(car_t *car);
void car_close(car_t *car);
void car_run_motor_ultrasonic(car_t *car, const float distance[3]);
void car_mode_ultrasonic(car_t *car, double now_s);
void car_mode_infrared(car_t *car, double now_s);
void car_mode_light(car_t *car, double now_s);
void car_mode_rotate(car_t *car, int angle_deg);

#endif
