#include <ESP32Servo.h>

// Ultrasonic Sensor Pins
#define TRIG_PIN 16
#define ECHO_PIN 15

// Servo Pin
#define SERVO_PIN 14

// Servo object
Servo radarServo;

// Variables
float duration, distance;

void setup() {
  Serial.begin(115200); // For sending data to Processing
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Attach servo
  radarServo.attach(SERVO_PIN, 500, 2400); // Min and max pulse widths in microseconds
}

void loop() {
  for (int angle = 0; angle <= 180; angle += 2) {
    radarServo.write(angle);  // Move servo to current angle
    delay(20);                // Allow servo to reach position
    distance = getDistance(); // Measure distance

    // Send data to Processing
    Serial.print(angle);
    Serial.print(",");
    Serial.println(distance);

    delay(50);
  }

  for (int angle = 180; angle >= 0; angle -= 2) {
    radarServo.write(angle);
    delay(20);
    distance = getDistance();

    Serial.print(angle);
    Serial.print(",");
    Serial.println(distance);

    delay(50);
  }
}

// Function to measure distance
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  return (duration * 0.034) / 2; // Convert to cm
}