#include <ArduinoRS485.h>  // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

// Define the DE/RE control pin for MAX485
#define RS485_CONTROL_PIN 2  // Connect DE and RE pins of MAX485 to this pin

const int registerAddress = 0; // Modbus register address

unsigned long lastRequestTime = 0; // Timestamp of the last Modbus request
const unsigned long idleTimeout = 5000; // Idle timeout in milliseconds (5 seconds)
bool idleMode = false;
bool requestReceived = false;

void setup() {
  // Initialize Serial for debugging (optional)
  Serial.begin(115200);
  while (!Serial); // Wait for Serial to be ready (for boards like Leonardo)

  // Initialize RS485 with control pin
  RS485.begin(115200); // Baud rate (115200 is good for long distances)
  RS485.setDelays(10, 10); // Optional: Pre- and post-transmission delays in microseconds

  // Start the Modbus RTU server with slave ID 1
  if (!ModbusRTUServer.begin(1, 115200)) {
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
  requestReceived = ModbusRTUServer.poll();
  if (requestReceived) {
    lastRequestTime = millis(); // Update the last request timestamp
    idleMode = false; // Reset idle mode
    // Optional: Debug output only if request received.
    Serial.println();
    Serial.print("Register value: ");
    Serial.print(value);
    Serial.println();
  }

  // Check for idle timeout
  if (millis() - lastRequestTime > idleTimeout) {
    idleMode = true; // Enter idle mode
  }

  delay(100); // Reduce delay to improve responsiveness
}