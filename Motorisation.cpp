#include "pico/stdlib.h"
#include "pico/time.h"
#include "Encoder.h"
#include "Wheel.h"
#include "Movements.h"
#include "PID.h"
#include <stdio.h>

static bool pid_timer_callback(repeating_timer_t* timer) {
  auto* movements = static_cast<Movements*>(timer->user_data);
  movements->UpdateControlLoop();
  return true;
}

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
  repeating_timer_t pidTimer;

  if (!add_repeating_timer_ms(-10, pid_timer_callback, &movements, &pidTimer)) {
    while (true) {
      sleep_ms(1000);
    }
  }

  movements.Rotate(270); 

  sleep_ms(5000);

  movements.MoveStraight(150, true);

  sleep_ms(5000);

  movements.Rotate(-270);

  sleep_ms(5000);

  movements.MoveStraight(255, false);

  sleep_ms(5000);

  movements.Stop();

  while (true) {
    sleep_ms(100);
  }
}
