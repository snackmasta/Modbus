#define TX_ENABLE_PIN 8

void setup() {
  pinMode(TX_ENABLE_PIN, OUTPUT);
  digitalWrite(TX_ENABLE_PIN, LOW); // Initialize in receive mode
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  if (Serial.available() > 0) {
    int receivedNumber = Serial.read(); // Read the received number
    Serial.print("Received Number: ");
    Serial.println(receivedNumber); // Print the received number to the Serial Monitor
  }
}
