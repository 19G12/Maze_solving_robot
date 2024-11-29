const int LMB = 13; // Left motor backward
const int LMF = 12; // Left motor forward
const int RMF = 11; // Right motor forward
const int RMB = 10; // Right motor backward

// Define sensor pins
const int rightmost = 4;
const int right = 3;
const int middle = 2;
const int left = 1;
const int leftmost = 0;
const int back = 7;

// Define global variables
int speed = 45000; // Speed variable (not used here due to lack of PWM in example)
int const TURN_DELAY = 2000;
int const BLACK = 1;
int const WHITE = 0;

// Setup method
void setup() {
  Serial.begin(9600);
  // Initialize motor pins as outputs
  pinMode(LMB, OUTPUT);
  pinMode(LMF, OUTPUT);
  pinMode(RMF, OUTPUT);
  pinMode(RMB, OUTPUT);

  // Initialize sensor pins as inputs
  pinMode(rightmost, INPUT);
  pinMode(right, INPUT);
  pinMode(middle, INPUT);
  pinMode(left, INPUT);
  pinMode(leftmost, INPUT);
  pinMode(back, INPUT);

  // Ensure motors are stopped initially
  stop();
}

// Loop method
void loop() {
  int sensorValues[6];
  lineFollowing(sensorValues); // Read sensor values

  
    if (sensorValues[0] == WHITE && sensorValues[1] == WHITE && sensorValues[2] == WHITE && sensorValues[5] == WHITE && sensorValues[3] == WHITE && sensorValues[4] == WHITE){
        stop();
      }
   else if (sensorValues[1] == WHITE && sensorValues[2] == BLACK && sensorValues[5] == BLACK && sensorValues[3] == WHITE) {
      forward();
    } 
  // Condition to move back
    else if (sensorValues[1] == WHITE && sensorValues[2] == WHITE && sensorValues[5] == BLACK && sensorValues[3] == WHITE){
        stop();
        turnBack();
      }
  // T-left or Left
    else if (sensorValues[1] == BLACK && (sensorValues[2] == BLACK || sensorValues[2] == WHITE) && sensorValues[3] == WHITE && sensorValues[4] == WHITE) {
        //Default
        stop();
        turnLeft();
      }
  // T-right
    else if (sensorValues[1] == WHITE && sensorValues[0] == WHITE && sensorValues[2] == BLACK && sensorValues[3] == BLACK){
        forward();
      }
  // Right 
     else if (sensorValues[0] == WHITE && sensorValues[1] == WHITE && sensorValues[2] == WHITE && sensorValues[3] == BLACK){
        stop();
        turnRight();
      }
   // All Black
      else if (sensorValues[0] == BLACK && sensorValues[1] == BLACK && sensorValues[2] == BLACK && sensorValues[3] == BLACK && sensorValues[4] == BLACK){
          stop();
          turn_left_45();
          // check for end condition
          if (sensorValues[2] == WHITE){
            stop();
            Serial.println("END");
            return; 
           }
           else {
             turn_left_45();
           }
        }
}

// Function to read sensor values
void lineFollowing(int sensorValues[]) {
  
  sensorValues[0] = digitalRead(leftmost);
  sensorValues[1] = digitalRead(left);
  sensorValues[2] = digitalRead(middle);
  sensorValues[3] = digitalRead(right);
  sensorValues[4] = digitalRead(rightmost);
  sensorValues[5] = digitalRead(back);
}

// Motor control functions
void forward() {
  Serial.println("Forward");
  digitalWrite(LMB, HIGH);
  digitalWrite(LMF, LOW);
  digitalWrite(RMF, HIGH);
  digitalWrite(RMB, LOW);
}

void turnRight() {
  Serial.println("Turn Right");
  digitalWrite(LMB, HIGH);
  digitalWrite(LMF, LOW);
  digitalWrite(RMF, LOW);
  digitalWrite(RMB, HIGH);
  stop();
}

void turnLeft() {
  Serial.println("Turn Left");
  digitalWrite(LMB, HIGH);
  digitalWrite(LMF, LOW);
  digitalWrite(RMF, LOW);
  digitalWrite(RMB, HIGH);
  stop();
}

void turnBack(){
  Serial.println("Turn Back");
  digitalWrite(LMB, HIGH);
  digitalWrite(LMF, LOW);
  digitalWrite(RMF, LOW);
  digitalWrite(RMB, HIGH);
  stop();
}

void turn_left_45() {
  Serial.println("Turn light Left");
  digitalWrite(LMB, HIGH);
  digitalWrite(LMF, LOW);
  digitalWrite(RMF, LOW);
  digitalWrite(RMB, HIGH);

  stop();
}

void stop() {
  Serial.println("Stop");
  digitalWrite(LMB, LOW);
  digitalWrite(LMF, LOW);
  digitalWrite(RMF, LOW);
  digitalWrite(RMB, LOW);
}
