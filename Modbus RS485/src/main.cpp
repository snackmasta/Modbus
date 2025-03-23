// Receiver Code
#include <Arduino.h>
#include <SoftwareSerial.h>
 
// Define the pins for the MAX485
#define DE 3
#define RE 2
 
// Create a SoftwareSerial object to communicate with the MAX485
SoftwareSerial RS485Serial(10, 11); // RX, TX
 
void setup() {
  // Initialize the serial communication
  Serial.begin(9600);
  RS485Serial.begin(9600);
 
  // Set the DE and RE pins as outputs
  pinMode(DE, OUTPUT);
  pinMode(RE, OUTPUT);
 
  // Set DE and RE low to enable receiving mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // Debug print
  Serial.println("Setup complete. Receiver ready.");
}
 
void loop() {
  // Debug print
  Serial.println("Loop started. Checking for data...");
  
  if (RS485Serial.available()) {
    // Read the received data
    int receivedData = RS485Serial.read();
 
    // Print the received data to the serial monitor
    Serial.print("Data received: ");
    Serial.println(receivedData);
 
    // Print a successful message
    Serial.println("Data successfully received.");
  } else {
    // Debug print
    Serial.println("No data available.");
  }

  // Add a small delay to avoid flooding the serial monitor
  delay(1000);
}