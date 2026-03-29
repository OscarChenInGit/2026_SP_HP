#ifndef SERVO_H
#define SERVO_H

int servo_init(void);
void servo_set_angle(int logical_channel, int angle_deg);
void servo_close(void);

#endif
