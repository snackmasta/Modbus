#include <Arduino.h>

void setup() {
  // Initialize serial communication at 9600 baud rate
  Serial.begin(9600);
}

void loop() {
  // Define the numerical value to send
  int value = 12345;

  // Send the value over UART
  Serial.println(value);

  // Wait for a second before sending the next value
  delay(1000);
}