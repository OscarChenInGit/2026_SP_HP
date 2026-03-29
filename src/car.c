#include "car.h"
#include "hw.h"
#include "motor.h"
#include "servo.h"
#include <math.h>
#include <stdio.h>
#include "util.h"

static void sleep_ms(long ms) {
    if (ms <= 0) return;
    double end = monotonic_seconds() + (double)ms / 1000.0;
    while (monotonic_seconds() < end) { }
}

int car_init(car_t *car) {
    if (!car) return -1;
    car->sonic_servo_angle = 30;
    car->sonic_servo_dir = 1;
    car->sonic_distance[0] = 30.0f;
    car->sonic_distance[1] = 30.0f;
    car->sonic_distance[2] = 30.0f;
    car->last_tick_s = 0.0;
    car->time_compensate = 3;
    servo_init();
    return motor_init();
}

void car_close(car_t *car) {
    (void)car;
    motor_close();
    servo_close();
}

void car_run_motor_ultrasonic(car_t *car, const float d[3]) {
    (void)car;
    if ((d[0] < 30 && d[1] < 30 && d[2] < 30) || d[1] < 30) {
        motor_set_model(-1450, -1450, -1450, -1450);
        sleep_ms(100);
        if (d[0] < d[2]) motor_set_model(1450, 1450, -1450, -1450);
        else motor_set_model(-1450, -1450, 1450, 1450);
    } else if (d[0] < 30 && d[1] < 30) {
        motor_set_model(1500, 1500, -1500, -1500);
    } else if (d[2] < 30 && d[1] < 30) {
        motor_set_model(-1500, -1500, 1500, 1500);
    } else if (d[0] < 20) {
        motor_set_model(2000, 2000, -500, -500);
        if (d[0] < 10) motor_set_model(1500, 1500, -1000, -1000);
    } else if (d[2] < 20) {
        motor_set_model(-500, -500, 2000, 2000);
        if (d[2] < 10) motor_set_model(-1500, -1500, 1500, 1500);
    } else {
        motor_set_model(600, 600, 600, 600);
    }
}

void car_mode_ultrasonic(car_t *car, double now_s) {
    if (!car || now_s - car->last_tick_s <= 0.2) return;
    car->last_tick_s = now_s;
    servo_set_angle(0, car->sonic_servo_angle);
    float dist = 0.0f;
    hw_ultrasonic_get_distance(&dist);
    if (car->sonic_servo_angle == 30) car->sonic_distance[0] = dist;
    else if (car->sonic_servo_angle == 90) car->sonic_distance[1] = dist;
    else if (car->sonic_servo_angle == 150) car->sonic_distance[2] = dist;
    car_run_motor_ultrasonic(car, car->sonic_distance);

    if (car->sonic_servo_angle <= 30) car->sonic_servo_dir = 1;
    else if (car->sonic_servo_angle >= 150) car->sonic_servo_dir = 0;
    car->sonic_servo_angle += car->sonic_servo_dir ? 60 : -60;
}

void car_mode_infrared(car_t *car, double now_s) {
    if (!car || now_s - car->last_tick_s <= 0.2) return;
    car->last_tick_s = now_s;
    int infrared_value = 0;
    hw_infrared_read_all(&infrared_value);
    switch (infrared_value) {
        case 2: motor_set_model(800, 800, 800, 800); break;
        case 4: motor_set_model(-1500, -1500, 2500, 2500); break;
        case 6: motor_set_model(-2000, -2000, 4000, 4000); break;
        case 1: motor_set_model(2500, 2500, -1500, -1500); break;
        case 3: motor_set_model(4000, 4000, -2000, -2000); break;
        case 7: motor_set_model(0, 0, 0, 0); break;
        default: break;
    }
}

void car_mode_light(car_t *car, double now_s) {
    if (!car || now_s - car->last_tick_s <= 0.2) return;
    car->last_tick_s = now_s;
    float l = 0.0f, r = 0.0f;
    hw_adc_read(0, &l);
    hw_adc_read(1, &r);
    motor_set_model(0, 0, 0, 0);
    if (l < 2.99f && r < 2.99f) {
        motor_set_model(600, 600, 600, 600);
    } else if (fabsf(l - r) < 0.15f) {
        motor_set_model(0, 0, 0, 0);
    } else if (l > 3.0f || r > 3.0f) {
        if (l > r) motor_set_model(-1200, -1200, 1400, 1400);
        else motor_set_model(1400, 1400, -1200, -1200);
    }
}

void car_mode_rotate(car_t *car, int angle_deg) {
    if (!car) return;
    float battery = 0.0f;
    hw_adc_read(2, &battery);
    float bat_comp = 7.5f / (battery * 2.0f);
    int angle = angle_deg;
    while (1) {
        int w = 2000;
        int vy = (int)(2000.0 * cos(angle * 3.14159265358979323846 / 180.0));
        int vx = (int)(-2000.0 * sin(angle * 3.14159265358979323846 / 180.0));
        int fr = vy - vx + w;
        int fl = vy + vx - w;
        int bl = vy - vx - w;
        int br = vy + vx + w;
        motor_set_model(fl, bl, fr, br);
        sleep_ms((long)(5 * car->time_compensate * bat_comp));
        angle -= 5;
    }
}
