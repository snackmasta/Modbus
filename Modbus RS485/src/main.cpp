#include <ArduinoRS485.h>  // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

// Define the DE/RE control pin for MAX485
#define RS485_CONTROL_PIN 2  // Connect DE and RE pins of MAX485 to this pin

const int registerAddress = 0; // Modbus register address

void setup() {
  // Initialize Serial for debugging (optional)
  Serial.begin(9600);
  while (!Serial); // Wait for Serial to be ready (for boards like Leonardo)

  // Initialize RS485 with control pin
  RS485.begin(9600); // Baud rate (9600 is good for long distances)
  RS485.setDelays(10, 10); // Optional: Pre- and post-transmission delays in microseconds

  // Start the Modbus RTU server with slave ID 1
  if (!ModbusRTUServer.begin(1, 9600)) {
    Serial.println("Failed to start Modbus RTU Server!");
    while (1);
  }

  // Configure a holding register at address 0
  ModbusRTUServer.configureHoldingRegisters(registerAddress, 1);

  // Set the DE/RE control pin (handled automatically by ArduinoRS485 if defined)
  pinMode(RS485_CONTROL_PIN, OUTPUT);
}

void loop() {
  // Read the value from the A0 analog pin
  int analogValue = analogRead(A0);

  // Map the analog value (0-1023) to the range 101-200
  int value = map(analogValue, 0, 1023, 101, 200);

  // Update the value in the holding register
  ModbusRTUServer.holdingRegisterWrite(registerAddress, value);

  // Poll for Modbus requests
  ModbusRTUServer.poll();

  // Optional: Debug output
  Serial.print("Register value: ");
  Serial.println(value);
  delay(1000); // Slow down for testing
}