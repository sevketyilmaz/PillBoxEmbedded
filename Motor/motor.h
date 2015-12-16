#ifndef __MOTOR_H
#define __MOTOR_H

extern int Direction,Motor_Status;

void motor_pin_init(void);
void solenoid_pin_init(void);
void stepper(void);
void stopMotor(void);
void SetDirection(void);
void motor_switch_pin_init(void);
void go_to_box(uint8_t current_boxnumber, uint8_t next_boxnumber);

#endif /* __MOTOR_H */
