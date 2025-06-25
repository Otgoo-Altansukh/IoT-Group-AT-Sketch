// Segment pins (A–G, DP)
const int segmentPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
// Digit cathodes (common)
const int digitPins[4] = {10, 11, 27, 26};

// Ultrasonic
#define TRIG_PIN 14
#define ECHO_PIN 15

// 0–9 segment maps
const byte digitMap[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

void setup() {
  for (int i = 0; i < 8; i++) pinMode(segmentPins[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(digitPins[i], OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(115200);
}

void loop() {
  float distance = getDistanceCM();
  int num = (int)distance; // cast to integer

  // Split digits
  int d1 = (num / 1000) % 10;
  int d2 = (num / 100) % 10;
  int d3 = (num / 10) % 10;
  int d4 = num % 10;

  // Display multiplexed
  showDigit(0, d1);
  showDigit(1, d2);
  showDigit(2, d3);
  showDigit(3, d4);
}

float getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.0343 / 2;
  Serial.println(distance);
  return distance;
}

void showDigit(int digitIndex, int number) {
  // Turn all digits off
  for (int i = 0; i < 4; i++) digitalWrite(digitPins[i], HIGH);

  // Set segments for number
  byte segments = digitMap[number];
  for (int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], (segments >> i) & 1);
  }

  // Turn on current digit (active LOW)
  digitalWrite(digitPins[digitIndex], LOW);
  delay(2); // short persistence
}
