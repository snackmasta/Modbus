#include <ArduinoRS485.h>  // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

// Define the DE/RE control pin for MAX485
#define RS485_CONTROL_PIN 2  // Connect DE and RE pins of MAX485 to this pin

const int registerAddress = 0; // Modbus register address
unsigned long lastRequestTime = 0; 
const unsigned long idleTimeout = 5000; // 5 seconds idle timeout
bool idleMode = false;
bool requestReceived = false;

// **Function Declarations**
void printModbusResponse(uint8_t slaveID, uint8_t functionCode, uint16_t regAddress, uint16_t data);
uint16_t calculateCRC(uint8_t *buffer, uint8_t length);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  RS485.begin(115200);
  RS485.setDelays(10, 10); 

  if (!ModbusRTUServer.begin(1, 115200)) {
    Serial.println("Failed to start Modbus RTU Server!");
    while (1);
  }

  ModbusRTUServer.configureHoldingRegisters(registerAddress, 1);
  pinMode(RS485_CONTROL_PIN, OUTPUT);
}

void loop() {
  int analogValue = analogRead(A0);
  int value = map(analogValue, 0, 1023, 101, 200);
  ModbusRTUServer.holdingRegisterWrite(registerAddress, value);

  requestReceived = ModbusRTUServer.poll();
  if (requestReceived) {
    lastRequestTime = millis();
    idleMode = false;

    Serial.println();
    Serial.print("Register updated: ");
    Serial.println(value);
    
    // Debugging: Print the expected Modbus response frame
    printModbusResponse(1, 0x03, registerAddress, value);
    Serial.println();
  }

  if (millis() - lastRequestTime > idleTimeout) {
    idleMode = true;
  }

  delay(100);
}

// **Function Definitions**
void printModbusResponse(uint8_t slaveID, uint8_t functionCode, uint16_t regAddress, uint16_t data) {
  uint8_t frame[8];

  frame[0] = slaveID;
  frame[1] = functionCode;
  frame[2] = 2;  // Byte count (2 bytes for 1 register)
  frame[3] = (data >> 8) & 0xFF; // High byte
  frame[4] = data & 0xFF; // Low byte

  uint16_t crc = calculateCRC(frame, 5);
  frame[5] = crc & 0xFF; // CRC Low Byte
  frame[6] = (crc >> 8) & 0xFF; // CRC High Byte

  for (int i = 0; i < 7; i++) {
    if (frame[i] < 0x10) Serial.print("0"); // Add leading zero if needed
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
}

uint16_t calculateCRC(uint8_t *buffer, uint8_t length) {
  uint16_t crc = 0xFFFF;
  for (uint8_t i = 0; i < length; i++) {
    crc ^= buffer[i];
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 1)
        crc = (crc >> 1) ^ 0xA001;
      else
        crc >>= 1;
    }
  }
  return crc;
}
