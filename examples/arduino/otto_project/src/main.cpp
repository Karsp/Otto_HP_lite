/*
  Otto Standby Base Sketch
  - Listens to all base Otto sensors + Sense Kit sensors
  - Calls independent functions for each sensor’s logic
  - Every 5 minutes calls react() to perform a mood-based action

  NOTE: You’ll need the Otto library (from HP/Otto) plus the
  relevant Sense‐kit libraries (e.g. DHT, Adafruit_Sensor, etc.).
  Adjust pin numbers and library includes as needed for your exact hardware.
*/

// --- Library Includes ---
#include <OttoDIYLib.h>                 // Otto base library
#include "../include/DHT/DHT.h"                  // Temperature & Humidity (Sense Kit)
#include <Wire.h>                 // I2C for accelerometer
#include "../include/adafruit_sensor/Adafruit_Sensor.h"      // Unified sensor base (for accelerometer)
#include <Adafruit_ADXL345_U.h>   // 3‐axis accelerometer (Sense Kit)

// --- Pin Definitions (adjust as needed) ---
#define DHT_PIN         A0        // DHT temperature/humidity pin (Sense)
#define DHT_TYPE        DHT11     // DHT11 or DHT22, whichever you have
#define SOUND_PIN       2         // Digital pin for sound/clap sensor (Sense)
#define LIGHT_PIN       A1        // Analog pin for photoresistor (Sense)
#define LINE_LEFT_PIN   A2        // Otto’s left line‐follower (base)
#define LINE_RIGHT_PIN  A3        // Otto’s right line‐follower (base)
#define OBSTACLE_TRIG   7         // HC‐SR04 trig (base)
#define OBSTACLE_ECHO   8         // HC‐SR04 echo (base)

//https://learn.adafruit.com/create-an-internet-of-things-dashboard-with-adafruit-dot-io/build-the-arduino-sketch
#define IO_USERNAME  "radmadrab"
#define IO_KEY       "aio_cXXs62XjiEh7xyrRYAI7aEyh46J

// --- Global Objects / Variables ---
Otto robot;                      // Otto instance

DHT dht(DHT_PIN, DHT_TYPE);       // Temp/Humidity sensor
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345); // Accelerometer

unsigned long lastReactTime = 0;  // Tracks when react() was last called
const unsigned long REACT_INTERVAL = 300000; // 5 minutes in milliseconds

// Forward declarations of all listener functions
void listenObstacle();
void listenLineFollow();
void listenSound();
void listenTempHumidity();
void listenLight();
void listenMotion();

void react();  // Called every 5 minutes

// -----------------------------------------------------------------------------
void setup() {
  // Initialize serial (for debugging)
  Serial.begin(45221);
  delay(100);

  // --- Otto Base Initialization ---
  robot.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_Buzzer);
//   robot.init();            // Initialize servos, LEDs, etc.
//   robot.leds("off");       // Start with LEDs off
	Serial.println("[LED off]");

  // --- Base Sensors Setup ---
  pinMode(LINE_LEFT_PIN, INPUT);
  pinMode(LINE_RIGHT_PIN, INPUT);
  pinMode(OBSTACLE_TRIG, OUTPUT);
  pinMode(OBSTACLE_ECHO, INPUT);

  // --- Sense Kit Initialization ---
  // 1. Temperature & Humidity
  dht.begin();

  // 2. Sound (clap) sensor
//   pinMode(SOUND_PIN, INPUT);

  // 3. Light (photoresistor)
  pinMode(LIGHT_PIN, INPUT);

  // 4. Accelerometer
  if (!accel.begin()) {
    Serial.println("ERROR: Could not detect ADXL345 accelerometer");
    // You might want to handle this error in your own way, e.g. blink LEDs
  }

  // Record the initial react time
  lastReactTime = millis();
}

// -----------------------------------------------------------------------------
void loop() {
  // 1. Continuously “listen” to each sensor
  listenObstacle();
  listenLineFollow();
//   listenSound();
  listenTempHumidity();
//   listenLight();
  listenMotion();

  // 2. Every REACT_INTERVAL ms, call react() exactly once
  unsigned long currentMillis = millis();
  if (currentMillis - lastReactTime >= REACT_INTERVAL) {
    lastReactTime = currentMillis;
    react();
  }

  // A small delay to avoid excessively tight looping
  delay(50);
}

// -----------------------------------------------------------------------------
/* listenObstacle()
   - Checks Otto’s ultrasonic obstacle sensor and calls a handler if something is too close.
   - You can expand this to trigger avoidance, alerting, or mood changes, etc. */
void listenObstacle() {
  // Trigger ultrasonic pulse
  digitalWrite(OBSTACLE_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(OBSTACLE_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(OBSTACLE_TRIG, LOW);

  // Read echo
  long duration = pulseIn(OBSTACLE_ECHO, HIGH);
  float distance_cm = (duration / 2.0) * 0.0343; // speed of sound ~0.0343 cm/us

  if (distance_cm > 0 && distance_cm < 15.0) { // threshold: 15cm
    // TODO: Call your custom obstacle‐detected handler here
    // e.g., handleObstacleDetected(distance_cm);
    Serial.print("Obstacle detected at ");
    Serial.print(distance_cm);
    Serial.println(" cm");
  }
}

// -----------------------------------------------------------------------------
/* listenLineFollow()
   - Reads the two line‐follower sensors (left/right). You can decide
     what to do if Otto is on/ off the line. */
void listenLineFollow() {
  int leftValue  = analogRead(LINE_LEFT_PIN);
  int rightValue = analogRead(LINE_RIGHT_PIN);

  // Example thresholds (adjust based on your calibration)
  const int THRESHOLD = 500;

  bool onLeftLine  = (leftValue  < THRESHOLD);
  bool onRightLine = (rightValue < THRESHOLD);

  if (onLeftLine || onRightLine) {
    // TODO: Call your custom line‐follow handler here
    // e.g., handleLineDetected(onLeftLine, onRightLine);
    Serial.print("Line sensor: L=");
    Serial.print(leftValue);
    Serial.print(" R=");
    Serial.println(rightValue);
  }
}

// -----------------------------------------------------------------------------
/* listenSound()
   - Listens for a “clap” or sound event (digital HIGH/LOW).
   - Expand this to change mood or trigger an action. */
void listenSound() {
  int soundState = digitalRead(SOUND_PIN);
  if (soundState == HIGH) {
    // TODO: Call your custom sound/clap handler here
    // e.g., handleClapDetected();
    Serial.println("Clap detected!");
    // Debounce: wait a little so you don’t retrigger immediately
    delay(200);
  }
}

// -----------------------------------------------------------------------------
/* listenTempHumidity()
   - Reads temperature & humidity from DHT; if their values cross thresholds,
     you can call a handler to react (e.g., change LED color). */
void listenTempHumidity() {
  float humidity = dht.readHumidity();
  float tempC    = dht.readTemperature(); // Celsius

  // Check if reading failed
  if (isnan(tempC) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Example thresholds:
  if (tempC > 30.0) {
    // TODO: handleHighTemperature(tempC);
    Serial.print("High temp: ");
    Serial.print(tempC);
    Serial.println(" °C");
  }
  if (humidity > 70.0) {
    // TODO: handleHighHumidity(humidity);
    Serial.print("High humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
}

// -----------------------------------------------------------------------------
/* listenLight()
   - Reads the analog photoresistor and triggers a handler if it’s too dark/bright. */
void listenLight() {
  int lightValue = analogRead(LIGHT_PIN);
  // Example thresholds (adjust based on ambient conditions):
  const int DARK_THRESHOLD = 300;
  const int BRIGHT_THRESHOLD = 700;

  if (lightValue < DARK_THRESHOLD) {
    // TODO: handleDark(lightValue);
    Serial.print("It’s dark (lightValue=");
    Serial.print(lightValue);
    Serial.println(")");
  }
  else if (lightValue > BRIGHT_THRESHOLD) {
    // TODO: handleBright(lightValue);
    Serial.print("It’s bright (lightValue=");
    Serial.print(lightValue);
    Serial.println(")");
  }
}

// -----------------------------------------------------------------------------
/* listenMotion()
   - Uses the ADXL345 accelerometer to detect shakes or movement.
   - If the acceleration magnitude crosses a threshold, call a handler. */
void listenMotion() {
  sensors_event_t event;
  accel.getEvent(&event);

  // Simple magnitude calculation: sqrt(x^2 + y^2 + z^2)
  float magnitude = sqrt(
    (event.acceleration.x * event.acceleration.x) +
    (event.acceleration.y * event.acceleration.y) +
    (event.acceleration.z * event.acceleration.z)
  );

  // Earth’s gravity ~9.8 m/s^2, so detect “shake” if magnitude > ~12
  if (magnitude > 12.0) {
    // TODO: handleShakeDetected(magnitude);
    Serial.print("Shake detected! accel magnitude = ");
    Serial.println(magnitude);
    // Debounce/delay so you don’t flood prints
    delay(200);
  }
}

// -----------------------------------------------------------------------------
/* react()
   - Called every 5 minutes (controlled by REACT_INTERVAL).
   - Inside here, you can check Otto’s “mood” (e.g., keep track of a global state
     or randomize) and perform actions (LED patterns, small movements, sounds, etc.).
   - For now, this is just a stub you can expand. */
void react() {
  // Example: Randomly pick a “mood” and act accordingly
  int mood = random(0, 3); // 0=happy, 1=sad, 2=curious

  switch (mood) {
    case 0:
      // Otto is happy: blink green eyes and wiggle
      Serial.println("[LED green]");
    //   robot.idle(500);      // hold for 500ms
	  delay(500);
      Serial.println("[LED off]");
      robot.jitter(5);      // small wiggle
      Serial.println("React: Happy mood");
      break;

    case 1:
      // Otto is sad: blink blue eyes slowly
    //   robot.leds("blue");
	Serial.println("[LED blue]");

      delay(1000);
      Serial.println("[LED off]");
      delay(1000);
      Serial.println("React: Sad mood");
      break;

    case 2:
      // Otto is curious: spin in place a little
	Serial.println("[LED yellow]");

    //   robot.turnLeft(2);    // turn ~60°, adjust as needed
	// replace robot.turnLeft(2);
	  robot.turn(LEFT, 2);
    //   robot.turnRight(2);   // turn back
      // replace robot.turnRight(2);
  	  robot.turn(RIGHT, 2);
  	  Serial.println("[LED off]");
      Serial.println("React: Curious mood");
      break;
  }
}
