#include "OttoLite.h"

// --- OttoMotor Implementation ---
OttoMotor::OttoMotor() : _offset(0) {}

void OttoMotor::begin(int pinL, int pinR) {
    _leftServo.attach(pinL);
    _rightServo.attach(pinR);
    stop();
}

void OttoMotor::move(int speedL, int speedR, int ms) {
    // Basic duty cycle calculation for continuous servos
    // 90 is usually stop, >90 forward, <90 backward
    _leftServo.write(90 + speedL);
    _rightServo.write(90 - speedR); // Mirrored for the other side
    delay(ms);
}

void OttoMotor::stop() {
    _leftServo.write(90);
    _rightServo.write(90);
}

// --- OttoBuzzer Implementation ---
OttoBuzzer::OttoBuzzer(int pin) : _pin(pin) {
    pinMode(_pin, OUTPUT);
}

void OttoBuzzer::beep(int freq, int duration) {
    tone(_pin, freq, duration);
}

void OttoBuzzer::playConnect() {
    beep(600, 100);
    delay(100);
    beep(800, 200);
}