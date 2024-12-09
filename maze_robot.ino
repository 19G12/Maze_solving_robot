const int RMN = 13; // Left motor backward
const int RMP = 12; // Left motor forward
const int LMN = 11; // Right motor forward
const int LMP = 10; // Right motor backward 

// Define sensor pins
const int rightmost = 0;
const int right = 1;
const int middle = 2;
const int left = 3;
const int leftmost = 4;
const int back = 7;

// Define global variables
int speed = 45000; // Speed variable (not used here due to lack of PWM in example)
int const TURN_DELAY = 2000;
int const BLACK = 1;
int const WHITE = 0;
int sensorValues[6];

// Setup method
void setup() {
  Serial.begin(9600);
  // Initialize motor pins as outputs
  pinMode(LMN, OUTPUT);
  pinMode(LMP, OUTPUT);
  pinMode(RMP, OUTPUT);
  pinMode(RMN, OUTPUT);

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
  lineFollowing(sensorValues); // Read sensor values

  Serial.print(sensorValues[0]);
  Serial.print(" ");
  Serial.print(sensorValues[1]);
  Serial.print(" ");
  Serial.print(sensorValues[2]);
  Serial.print(" ");
  Serial.print(sensorValues[3]);
  Serial.print(" ");
  Serial.print(sensorValues[4]);
  Serial.print(" ");
  Serial.println(sensorValues[5]);

  
    if ((sensorValues[0] && sensorValues[1]  && !sensorValues[3] && !sensorValues[4] && sensorValues[5]) || (sensorValues[0] && sensorValues[1] && !sensorValues[2]  && sensorValues[3] && sensorValues[4] && sensorValues[5])){
        stop();
          delay(50);
          turnLeft();
          while (!(sensorValues[0] && sensorValues[1] && sensorValues[2] && (!sensorValues[5] || sensorValues[5])) ){
            Serial.println("Turning left");
            lineFollowing(sensorValues);
            delay(10);
           }
          forward();
          while ((sensorValues[0] && sensorValues[1] && sensorValues[2] && (!sensorValues[3] || sensorValues[3])) ){
            Serial.println("Forward");
            lineFollowing(sensorValues);
           }
    }
    else if(!sensorValues[0] && !sensorValues[1]  && sensorValues[2]  && sensorValues[5]  &&(( !sensorValues[3] && !sensorValues[4]) || ( sensorValues[3] && sensorValues[4]))){
        forward();
    }
    else if (!sensorValues[0] && !sensorValues[1] && !sensorValues[2] && sensorValues[5] && !sensorValues[3] && !sensorValues[4]) {
      stop();
      delay(250);
      turnBack();
      delay(2000);
      stop();
    }
    else if(!sensorValues[0] && !sensorValues[1] && !sensorValues[2] && sensorValues[5] && sensorValues[3] && sensorValues[4]){
        stop();
        delay(50);
        
        turnRight();
        while (!(!sensorValues[0] && !sensorValues[1] && sensorValues[2] && sensorValues[5] && sensorValues[3] && sensorValues[4]) ){
          Serial.println("Turning right");
          lineFollowing(sensorValues);
          delay(10);
         }
        forward();
        while ((!sensorValues[0] && !sensorValues[1] && sensorValues[2] && sensorValues[5] && sensorValues[3] && sensorValues[4])){
          Serial.println("Forward");
          lineFollowing(sensorValues);
         }
    }
    else {
        stop();
        delay(500);
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
  digitalWrite(LMN, LOW);
  digitalWrite(LMP, HIGH);
  digitalWrite(RMP, HIGH);
  digitalWrite(RMN, LOW);
}

void turnRight() {
  Serial.println("Turn Right");
  digitalWrite(LMP, HIGH);
  digitalWrite(LMN, LOW);
  digitalWrite(RMP, LOW);
  digitalWrite(RMN, HIGH);
}

void turnLeft() {
  Serial.println("Turn Left");
  digitalWrite(LMN, HIGH);
  digitalWrite(LMP, LOW);
  digitalWrite(RMP, HIGH);
  digitalWrite(RMN, LOW);
}

void turnBack(){
  Serial.println("Turn Back");
  digitalWrite(LMN, HIGH);
  digitalWrite(LMP, LOW);
  digitalWrite(RMP, HIGH);
  digitalWrite(RMN, LOW);
}

void stop() {
  Serial.println("Stop");
  digitalWrite(LMN, LOW);
  digitalWrite(LMP, LOW);
  digitalWrite(RMP, LOW);
  digitalWrite(RMN, LOW);
}
