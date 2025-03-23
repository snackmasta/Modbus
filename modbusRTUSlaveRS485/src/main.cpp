#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>
#include <SoftwareSerial.h>

// Define the pins for the MAX485
#define DE 3
#define RE 2

// Create a SoftwareSerial object to communicate with the MAX485
SoftwareSerial RS485Serial(10, 11); // RX, TX

const int registerAddress = 0; // Modbus register address
int value = 12345; // Value to send

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  RS485Serial.begin(9600);

  // Set the DE and RE pins as outputs
  pinMode(DE, OUTPUT);
  pinMode(RE, OUTPUT);

  // Set DE and RE high to enable transmission mode for initial sender config.
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);

  // Start the Modbus RTU server
  if (!ModbusRTUServer.begin(1, 9600)) {
    Serial.println("Failed to start Modbus RTU Server!");
    while (1);
  }

  // Configure a holding register at address 0
  ModbusRTUServer.configureHoldingRegisters(registerAddress, 1);

  // Set DE and RE low for receiver mode after modbus server starts.
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
}

void loop() {
  // Modbus Server Part (Receiver)
  ModbusRTUServer.holdingRegisterWrite(registerAddress, value);
  ModbusRTUServer.poll();

  // Sender Part
  // Generate random data
  int data = random(0, 100);

  // Enable transmission mode for sender
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);

  // Send data over RS485
  RS485Serial.write(data);

  // Print the sent data to the serial monitor
  Serial.print("Data sent: ");
  Serial.println(data);

  // Disable transmission mode for sender, enable receiver mode.
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // Wait for a while before sending the next data
  delay(2000);
}