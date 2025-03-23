#include <ArduinoRS485.h>  // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

// Define RS485 control pin
#define RS485_ENABLE_PIN 3

// Define Modbus slave address
#define SLAVE_ADDRESS 1

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT_PULLUP);
  pinMode(RS485_ENABLE_PIN, OUTPUT);

  // Start the Modbus RTU server
  if (!ModbusRTUServer.begin(SLAVE_ADDRESS, 9600)) {
    Serial.println("Failed to start Modbus RTU Server!");
    while (1);
  }

  // Configure holding register
  ModbusRTUServer.configureHoldingRegisters(0, 1);
  Serial.println("Modbus RTU Slave is ready");
}

void loop() {
  // Poll for Modbus RTU requests
  ModbusRTUServer.poll();

  static bool buttonPressed = false;
  int buttonState = digitalRead(8);

  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true;
    delay(20); // Basic debounce

    // Set the value of holding register 0 to 1234 when the button is pressed
    ModbusRTUServer.holdingRegisterWrite(0, 1234);
  } else if (buttonState == HIGH) {
    buttonPressed = false;
  }
}