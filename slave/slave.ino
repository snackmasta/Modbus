#include <ModbusRTUSlave.h>

// Define Modbus slave address
const byte slaveAddress = 1;

// Define RS485 control pin
const byte rs485EnablePin = 3;

// Define ModbusRTUSlave object
ModbusRTUSlave slave(Serial, rs485EnablePin);

// Variable to store the mapped potentiometer value
int mappedValue = 0;

void setup() {
  Serial.begin(9600);
  slave.begin(slaveAddress, 9600);
  // Configure one holding register starting at address 0
  slave.configureHoldingRegisters(0, 1);
}

void loop() {
  // Read potentiometer value
  int sensorValue = analogRead(A0);
  // Map the value to the desired range
  mappedValue = map(sensorValue, 0, 1023, 101, 200);
  // Update the holding register with the new value
  slave._holdingRegisters[0] = mappedValue; // Corrected line
  // Listen for Modbus requests and respond
  slave.poll();
}