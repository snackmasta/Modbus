#include <ArduinoRS485.h>  // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

// Define the DE/RE control pin for MAX485
#define RS485_CONTROL_PIN 2  // Connect DE and RE pins of MAX485 to this pin
#define BUTTON_PIN 8         // Define the button pin

const int registerAddress = 0; // Modbus register address

// Function Prototypes
void printModbusResponse(uint8_t slaveID, uint8_t functionCode, uint16_t data);
uint16_t calculateCRC(uint8_t *buffer, uint8_t length);

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
      // If the request succeeds, read and print the response frame
      while (ModbusRTUClient.available()) {
        int value = ModbusRTUClient.read();
        Serial.print("Received value: ");
        Serial.println(value);
        printModbusResponse(1, 0x03, value);  // Print Modbus response frame
      }
    }

    // Wait for a second before the next request
    delay(1000);
  }
}

// Function to print the Modbus response frame
void printModbusResponse(uint8_t slaveID, uint8_t functionCode, uint16_t data) {
  uint8_t frame[7];

  frame[0] = slaveID;
  frame[1] = functionCode;
  frame[2] = 2;  // Byte count (2 bytes for 1 register)
  frame[3] = (data >> 8) & 0xFF; // High byte
  frame[4] = data & 0xFF; // Low byte

  uint16_t crc = calculateCRC(frame, 5);
  frame[5] = crc & 0xFF; // CRC Low Byte
  frame[6] = (crc >> 8) & 0xFF; // CRC High Byte

  Serial.print("Response Frame: ");
  for (int i = 0; i < 7; i++) {
    if (frame[i] < 0x10) Serial.print("0");
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

// Function to calculate CRC16 for Modbus RTU
uint16_t calculateCRC(uint8_t *buffer, uint8_t length) {
  uint16_t crc = 0xFFFF;
  for (uint8_t i = 0; i < length; i++) {
    crc ^= buffer[i];
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x0001) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}
