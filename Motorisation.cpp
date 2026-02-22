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
  const uint LEFT_WHEEL_PWM1_PIN = 2;
  const uint LEFT_WHEEL_PWM2_PIN = 3;
  const uint RIGHT_WHEEL_PWM1_PIN = 4;
  const uint RIGHT_WHEEL_PWM2_PIN = 5;

  Encoder leftEncoder(LEFT_ENCODER_PIN);
  Encoder rightEncoder(RIGHT_ENCODER_PIN);
  PID leftPID(2.0f, 10.0f, 0.0f, 0.01f, 0.0f, 255.0f);
  PID rightPID(2.0f, 10.0f, 0.0f, 0.01f, 0.0f, 255.0f);
  Wheel leftWheel(LEFT_WHEEL_PWM1_PIN, LEFT_WHEEL_PWM2_PIN, leftEncoder, leftPID);
  Wheel rightWheel(RIGHT_WHEEL_PWM1_PIN, RIGHT_WHEEL_PWM2_PIN, rightEncoder, rightPID);

  Movements movements(leftWheel, rightWheel);

  while (true) {
    // movements.MoveStraight(100, true); // Move forward at speed 255
    movements.Rotate(90); // Rotate 90 degrees to the right

    sleep_ms(10);
  }
}
