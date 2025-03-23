#include <ArduinoRS485.h>  // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

// Define the DE/RE control pin for MAX485
#define RS485_CONTROL_PIN 2  // Connect DE and RE pins of MAX485 to this pin
#define BUTTON_PIN 8         // Define the button pin

const int registerAddress = 0; // Modbus register address

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  while (!Serial); // Wait for Serial to be ready (for boards like Leonardo)

  // Initialize RS485 with control pin
  RS485.begin(115200); // Baud rate (115200 is good for long distances)
  RS485.setDelays(10, 10); // Optional: Pre- and post-transmission delays in microseconds

  // Start the Modbus RTU client
  if (!ModbusRTUClient.begin(115200)) {
    Serial.println("Failed to start Modbus RTU Client!");
    while (1);
  }

  // Set the DE/RE control pin
  pinMode(RS485_CONTROL_PIN, OUTPUT);

  // Set the button pin as input
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Use internal pull-up resistor
}

void loop() {
  // Check if the button is pressed
  if (digitalRead(BUTTON_PIN) == HIGH) {
    // Read the value from the holding register of server with ID 1
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
}