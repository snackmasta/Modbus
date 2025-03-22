#include <ArduinoModbus.h>

void setup() {
    Serial.begin(9600);  // Debugging
    Serial1.begin(9600, SERIAL_8N1); // Use Serial1 for UART communication
    
    if (!ModbusRTUMaster.begin(Serial1)) {
        Serial.println("Failed to start Modbus RTU Master");
        while (1);
    }
}

void loop() {
    Serial.println("Requesting data from Slave...");

    uint16_t value;
    if (ModbusRTUMaster.requestFrom(1, HOLDING_REGISTERS, 0x00, 1)) { // Slave ID = 1, Register 0x00
        value = ModbusRTUMaster.read();
        Serial.print("Received Value: ");
        Serial.println(value);
    } else {
        Serial.println("Failed to read data!");
    }

    delay(1000);  // Wait before next request
}
