const int RMN = 10; // Left motor backward
const int RMP = 11; // Left motor forward
const int LMN = 12; // Right motor forward
const int LMP = 13; // Right motor backward 

// Define sensor pins
const int rightmost = 0;
const int right = 1;
const int middle = 7;
const int left = 3;
const int leftmost = 4;
const int back = 2;

// Define global variables
int const TURN_DELAY = 2000;
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
  lineFollowing(sensorValues); 

  
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

    // Handle: Left + Left-T + T
    if ((sensorValues[0] && sensorValues[1]  && !sensorValues[3] && !sensorValues[4] && sensorValues[5]) || (sensorValues[0] && sensorValues[1] && !sensorValues[2]  && sensorValues[3] && sensorValues[4] && sensorValues[5])){
        stop();
          delay(50);
          turnLeft();
          while (!(sensorValues[0] && sensorValues[1] && sensorValues[2])){
            Serial.println("Turning left");
            lineFollowing(sensorValues);
            delay(10);
           }
          forward();
          delay(100);
          while (sensorValues[0] && sensorValues[1] && sensorValues[2]){
            Serial.println("Forward");
            lineFollowing(sensorValues);
           }
    }
    // Handle: Straight + T-right
    else if(!sensorValues[0] && !sensorValues[1]  && sensorValues[2]  && sensorValues[5]  &&(( !sensorValues[3] && !sensorValues[4]) || ( sensorValues[3] && sensorValues[4]))){
        forward();
    }
    else if (!sensorValues[0] && !sensorValues[1] && !sensorValues[2] && sensorValues[5] && !sensorValues[3] && !sensorValues[4]) {
      // Change here
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
  
  sensorValues[0] = !digitalRead(leftmost);
  sensorValues[1] = !digitalRead(left);
  sensorValues[2] = !digitalRead(middle);
  sensorValues[3] = !digitalRead(right);
  sensorValues[4] = !digitalRead(rightmost);
  sensorValues[5] = !digitalRead(back);

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

void stop() {
  Serial.println("Stop");
  digitalWrite(LMN, LOW);
  digitalWrite(LMP, LOW);
  digitalWrite(RMP, LOW);
  digitalWrite(RMN, LOW);
}
