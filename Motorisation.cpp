#include "pico/stdlib.h"
#include "Encoder.h"
#include "Wheel.h"
#include "Movements.h"
#include "PID.h"
#include <stdio.h>

int main() {
  stdio_init_all();

  const uint LEFT_ENCODER_PIN = 0;
  const uint RIGHT_ENCODER_PIN = 1;
  const uint LEFT_WHEEL_PWM_PIN = 2;
  const uint LEFT_WHEEL_DIR_PIN = 3;
  const uint RIGHT_WHEEL_PWM_PIN = 4;
  const uint RIGHT_WHEEL_DIR_PIN = 5;

  Encoder leftEncoder(LEFT_ENCODER_PIN);
  Encoder rightEncoder(RIGHT_ENCODER_PIN);
  PID leftPID(2.0f, 10.0f, 0.0f, 0.01f, 0.0f, 255.0f);
  PID rightPID(2.0f, 10.0f, 0.0f, 0.01f, 0.0f, 255.0f);
  Wheel leftWheel(LEFT_WHEEL_PWM_PIN, LEFT_WHEEL_DIR_PIN, leftEncoder, leftPID);
  Wheel rightWheel(RIGHT_WHEEL_PWM_PIN, RIGHT_WHEEL_DIR_PIN, rightEncoder, rightPID);

  // Create PID controllers (kp, ki, kd, dt, u_min, u_max)
  // todo: tune these parameters
  // todo: set a PID for each wheel
  PID pid(2.0f, 0.1f, 0.0f, 0.01f, 0.0f, 255.0f);

  Movements movements(leftWheel, rightWheel, pid);

  while (true) {
    movements.MoveStraight(100); // Move forward at speed 255

    sleep_ms(10);
  }
}
