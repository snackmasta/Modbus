#include <ArduinoRS485.h>  // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

const int registerAddress = 0; // Modbus register address

void setup() {
  Serial.begin(9600);

  // Start the Modbus RTU client
  if (!ModbusRTUClient.begin(9600)) {
    Serial.println("Failed to start Modbus RTU Client!");
    while (1);
  }
}

void loop() {
  // Read the value from the holding register
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, registerAddress, 1)) {
    Serial.print("Failed to read holding register! ");
    Serial.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, print the value
    while (ModbusRTUClient.available()) {
      int value = ModbusRTUClient.read();
      Serial.print("Received value: ");
      Serial.println(value);
    }
  }

  // Wait for a second before the next request
  delay(1000);
}