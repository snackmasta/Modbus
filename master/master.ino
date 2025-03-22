#include <ArduinoRS485.h>  // Arduino RS485 library
#include <ArduinoModbus.h> // Arduino Modbus library

const int slaveID = 1;          // Modbus slave address
const int registerAddress = 0;  // Address of the holding register to write
const int valueToWrite = 123;   // Value to write to the holding register

void setup() {
  // Start the serial monitor for debugging
  Serial.begin(9600);
  while (!Serial);

  // Start the Modbus RTU client
  if (!ModbusRTUClient.begin(9600)) {
    Serial.println("Failed to start Modbus RTU Client!");
    while (1);
  }
}

void loop() {
  // Write a single holding register
  if (!ModbusRTUClient.holdingRegisterWrite(slaveID, registerAddress, valueToWrite)) {
    Serial.print("Failed to write holding register! Error: ");
    Serial.println(ModbusRTUClient.lastError());
  } else {
    Serial.println("Holding register written successfully!");
  }

  // Wait for 1 second before the next write
  delay(1000);
}