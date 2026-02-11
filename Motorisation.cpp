#include "pico/stdlib.h"
#include "Encoder.h"
#include "Motor.h"
#include "Movements.h"
#include "PID.h"
#include <stdio.h>

int main() {
  stdio_init_all();

  const uint LEFT_ENCODER_PIN = 0;
  const uint RIGHT_ENCODER_PIN = 1;
  const uint MOTOR1_PWM_PIN = 2;
  const uint MOTOR1_DIR_PIN = 3;
  const uint MOTOR2_PWM_PIN = 5;
  const uint MOTOR2_DIR_PIN = 4;

  Encoder leftEncoder(LEFT_ENCODER_PIN);
  Encoder rightEncoder(RIGHT_ENCODER_PIN);
  Motor motor1(MOTOR1_PWM_PIN, MOTOR1_DIR_PIN);
  Motor motor2(MOTOR2_PWM_PIN, MOTOR2_DIR_PIN);

  // Create PID controllers (kp, ki, kd, dt, u_min, u_max)
  // todo: tune these parameters
  // todo: set a PID for each wheel
  PID pid(0.8f, 1000.0f, 0.0f, 0.1f, 0.0f, 255.0f);

  Movements movements(motor1, motor2, leftEncoder, rightEncoder, pid);

  while (true) {
    movements.MoveStraight(180); // Move forward at speed 180

    sleep_ms(100);
  }
}
