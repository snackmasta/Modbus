#include <ArduinoModbus.h>

ModbusRTUSlave slave;

void setup() {
    Serial.begin(9600);  // Debugging
    Serial1.begin(9600, SERIAL_8N1);  // UART for Modbus RTU

    slave.begin(1, Serial1); // Slave ID = 1
    slave.configureHoldingRegisters(0x00, 1); // 1 holding register
    slave.holdingRegisterWrite(0x00, 1234); // Initial value in register
}

void loop() {
    slave.poll();  // Listen for Modbus requests
}
