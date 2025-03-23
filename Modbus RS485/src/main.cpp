#include <ModbusMaster.h>

// Define RS485 control pin
#define RS485_ENABLE_PIN 3

// Define Modbus slave address of the master
#define MASTER_SLAVE_ADDRESS 1

// Create ModbusMaster object
ModbusMaster node;

void preTransmission() {
  digitalWrite(RS485_ENABLE_PIN, HIGH);
}

void postTransmission() {
  digitalWrite(RS485_ENABLE_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT_PULLUP);
  pinMode(RS485_ENABLE_PIN, OUTPUT);
  // Initialize ModbusMaster node
  node.begin(MASTER_SLAVE_ADDRESS, Serial);
  // Set the transmit and receive functions for the ModbusMaster library
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  Serial.println("Modbus RTU Master is ready");
}

void loop() {
  static bool buttonPressed = false;
  int buttonState = digitalRead(8);

  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true;
    delay(20); // Basic debounce

    // Read holding register 0 from the master
    uint8_t result = node.readHoldingRegisters(0, 1);

    if (result == node.ku8MBSuccess) {
      Serial.print("Received value from master: ");
      Serial.println(node.getResponseBuffer(0));
    } else {
      Serial.print("Error reading from master: ");
      Serial.println(result);
    }
  } else if (buttonState == HIGH) {
    buttonPressed = false;
  }

  delay(10);
}