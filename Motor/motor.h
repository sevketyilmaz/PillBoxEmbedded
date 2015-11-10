#ifndef __MOTOR_H
#define __MOTOR_H

extern void motor_pin_init(void);
extern void solenoid_pin_init(void);
extern void stepper(void);
extern void stopMotor(void);
extern void SetDirection(void);

#endif /* __MOTOR_H */
