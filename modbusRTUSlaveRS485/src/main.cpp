#include <Arduino.h>

void setup() {
  // Initialize serial communication at 9600 baud rate
  Serial.begin(9600);
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming data as a string
    String receivedData = Serial.readStringUntil('\n');

    // Convert the received string to an integer
    int value = receivedData.toInt();

    // Print the received value to the Serial Monitor
    Serial.print("Received value: ");
    Serial.println(value);
  }
}