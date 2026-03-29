#ifndef MOTOR_H
#define MOTOR_H

int motor_init(void);
void motor_close(void);
void motor_set_model(int duty1, int duty2, int duty3, int duty4);

#endif
