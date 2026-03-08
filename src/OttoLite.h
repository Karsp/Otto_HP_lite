#ifndef OTTO_LITE_H
#define OTTO_LITE_H

#include <Arduino.h>
#include <ESP32Servo.h> // You will need to install the "ESP32Servo" library in Arduino IDE

class OttoMotor {
  private:
    Servo _leftServo;
    Servo _rightServo;
    int _offset;

  public:
    OttoMotor();
    void begin(int pinL, int pinR);
    void move(int speedL, int speedR, int ms);
    void stop();
};

class OttoBuzzer {
  private:
    int _pin;

  public:
    OttoBuzzer(int pin);
    void beep(int freq, int duration);
    void playConnect();
};

#endif