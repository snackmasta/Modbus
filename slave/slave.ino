#include <ArduinoRS485.h>  // Arduino RS485 library
#include <ArduinoModbus.h> // Arduino Modbus library

const int registerAddress = 0; // Address of the holding register to write
int holdingRegister = 0;       // Variable to store the holding register value

void setup() {
  // Start the serial monitor for debugging
  Serial.begin(9600);
  while (!Serial);

  // Start the Modbus RTU server with slave ID 1
  if (!ModbusRTUServer.begin(1, 9600)) {
    Serial.println("Failed to start Modbus RTU Server!");
    while (1);
  }

  // Configure holding register at address 0
  ModbusRTUServer.configureHoldingRegisters(registerAddress, 1);
}

void loop() {
  // Poll for Modbus RTU requests
  ModbusRTUServer.poll();

  // Read the value of the holding register
  int newValue = ModbusRTUServer.holdingRegisterRead(registerAddress);

  // If the holding register value has changed, print it
  if (newValue != holdingRegister) {
    holdingRegister = newValue;
    Serial.print("Holding register updated to: ");
    Serial.println(holdingRegister);
  }
}