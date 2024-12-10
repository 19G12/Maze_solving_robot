// Motor Pins
const int RMN = 13; // Left motor backward
const int RMP = 12; // Left motor forward
const int LMN = 11; // Right motor forward
const int LMP = 10; // Right motor backward

// Sensor Pins
const int rightmost = 0;
const int right = 1;
const int middle = 7;
const int left = 3;
const int leftmost = 4;
const int back = 2;

// Global Variables
const int motorSpeed = 150; // PWM speed (0-255)
int sensorValues[6];

// Constants
const int TURN_DELAY = 200; // Delay for turning

// Function Prototypes
void forward();
void stopMotors();
void turnRight();
void turnLeft();
void turnBack();
void readSensorValues();
bool isStraightLineDetected();
bool isLeftTurnRequired();
void printSensorStates();

void setup() {
  Serial.begin(9600);

  // Motor pins as outputs
  pinMode(LMN, OUTPUT);
  pinMode(LMP, OUTPUT);
  pinMode(RMP, OUTPUT);
  pinMode(RMN, OUTPUT);

  // Sensor pins as inputs
  pinMode(rightmost, INPUT);
  pinMode(right, INPUT);
  pinMode(middle, INPUT);
  pinMode(left, INPUT);
  pinMode(leftmost, INPUT);
  pinMode(back, INPUT);

  stopMotors(); // Ensure motors are stopped initially
}

void loop() {
  readSensorValues();
  printSensorStates();

  // Line following logic
  if (isStraightLineDetected()) {
    forward();
  } else if (isLeftTurnRequired()) {
    stopMotors();
    delay(TURN_DELAY);
    turnLeft();
    waitForCondition(isStraightLineDetected);
  } else if (sensorValues[5]) { // Back sensor detects the line
    stopMotors();
    delay(250);
    turnBack();
    delay(2000);
    stopMotors();
  } else {
    stopMotors(); // Stop if no valid condition is met
    delay(500);
  }
}

// Function to read sensor values (Digital)
void readSensorValues() {
  sensorValues[0] = digitalRead(leftmost);
  sensorValues[1] = digitalRead(left);
  sensorValues[2] = digitalRead(middle);
  sensorValues[3] = digitalRead(right);
  sensorValues[4] = digitalRead(rightmost);
  sensorValues[5] = digitalRead(back);
}

// Utility Functions for Motor Control
void forward() {
  Serial.println("Moving Forward");
  analogWrite(LMP, motorSpeed);
  analogWrite(RMP, motorSpeed);
  digitalWrite(LMN, LOW);
  digitalWrite(RMN, LOW);
}

void stopMotors() {
  Serial.println("Stopping");
  digitalWrite(LMN, LOW);
  digitalWrite(LMP, LOW);
  digitalWrite(RMP, LOW);
  digitalWrite(RMN, LOW);
}

void turnRight() {
  Serial.println("Turning Right");
  analogWrite(LMP, motorSpeed);
  digitalWrite(LMN, LOW);
  analogWrite(RMP, 0);
  analogWrite(RMN, motorSpeed);
}

void turnLeft() {
  Serial.println("Turning Left");
  analogWrite(LMP, 0);
  analogWrite(LMN, motorSpeed);
  analogWrite(RMP, motorSpeed);
  digitalWrite(RMN, LOW);
}

void turnBack() {
  Serial.println("Turning Back");
  analogWrite(LMP, motorSpeed);
  digitalWrite(LMN, LOW);
  analogWrite(RMP, motorSpeed);
  digitalWrite(RMN, LOW);
}

// Condition Check Helper
bool isStraightLineDetected() {
  return sensorValues[2] && (sensorValues[1] || sensorValues[3]);
}

bool isLeftTurnRequired() {
  return sensorValues[0] && sensorValues[1] && !sensorValues[2];
}

// Debugging Helper
void printSensorStates() {
  Serial.print("Sensors: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.println();
}

// Wait for a specific condition with timeout
void waitForCondition(bool (*condition)()) {
  unsigned long startTime = millis();
  while (!condition()) {
    if (millis() - startTime > 1000) { // Timeout after 1 second
      Serial.println("Timeout waiting for condition");
      stopMotors();
      break;
    }
    readSensorValues();
    delay(10);
  }
}
