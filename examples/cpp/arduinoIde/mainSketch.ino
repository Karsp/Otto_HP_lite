#include <OttoLite.h> // The IDE should now find this in your libraries folder

// Connector #10 (Left) and #11 (Right) are GPIO 14 and 13
OttoMotor motor;
OttoBuzzer buzzer(25); // Integrated Buzzer

void setup() {
    Serial.begin(115200);
    motor.begin(14, 13);
    
    buzzer.playConnect();
    Serial.println("Otto Lite Ready!");
}

void loop() {
    // Forward
    motor.move(20, 20, 1000); 
    motor.stop();
    delay(1000);
    
    // Backward
    motor.move(-20, -20, 1000);
    motor.stop();
    delay(1000);
}