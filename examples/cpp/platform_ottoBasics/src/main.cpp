#include <Arduino.h>
#include "OttoLite.h" // Ensure this is in your src folder

OttoMotor motors;

void setup() {
    // UPDATED PINS: 14 for Left, 13 for Right
    motors.begin(14, 13); 
}

void loop() {
    motors.move(20, 20, 1000); // Both wheels should now turn
    motors.stop();
    delay(1000);
}