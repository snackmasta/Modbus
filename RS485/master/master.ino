#define TX_ENABLE_PIN 8

void setup() {
  pinMode(TX_ENABLE_PIN, OUTPUT);
  digitalWrite(TX_ENABLE_PIN, LOW); // Initialize in receive mode
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  int numberToSend = 123; // Example number to send
  digitalWrite(TX_ENABLE_PIN, HIGH); // Enable transmission
  Serial.write(numberToSend); // Send the number
  delay(10); // Short delay
  digitalWrite(TX_ENABLE_PIN, LOW); // Disable transmission (enable reception)
  delay(1000); // Wait before sending the next number
}