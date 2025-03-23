#include <ArduinoRS485.h>  // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

const int registerAddress = 0; // Modbus register address
int value = 12345; // Value to send

void setup() {
  // Start the Modbus RTU server
  if (!ModbusRTUServer.begin(1, 9600)) {
    Serial.println("Failed to start Modbus RTU Server!");
    while (1);
  }

  // Configure a holding register at address 0
  ModbusRTUServer.configureHoldingRegisters(registerAddress, 1);
}

void loop() {
  // Update the value in the holding register
  ModbusRTUServer.holdingRegisterWrite(registerAddress, value);

  // Poll for Modbus requests
  ModbusRTUServer.poll();
}