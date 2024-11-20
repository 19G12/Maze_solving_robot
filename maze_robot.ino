#include <AFMotor.h>

// Sensor and Motor Pins (Digital Pins for Raspberry Pi Pico)
const int sensorV1 = 2;  // Vertical bottom
const int sensorV2 = 3;  // Vertical top
const int sensorH1 = 4;  // Leftmost horizontal
const int sensorH2 = 5;  // Inner left horizontal
const int sensorH3 = 6;  // Inner right horizontal
const int sensorH4 = 7;  // Rightmost horizontal
const int proximitySensorPin = 8; // IR proximity sensor pin

// Motor setup
AF_DCMotor motorLeft(3);  // Left motor (M3)
AF_DCMotor motorRight(4); // Right motor (M4)

// Robot class
class MazeRobot {
  public:
    int v1, v2, h1, h2, h3, h4; // Sensor readings
    bool obstacleDetected;      // Proximity sensor reading
    String path;                // Path log

    MazeRobot() {
      path = "";
    }

    void readSensors() {
      // Read digital sensors (HIGH = 1, LOW = 0)
      v1 = digitalRead(sensorV1);
      v2 = digitalRead(sensorV2);
      h1 = digitalRead(sensorH1);
      h2 = digitalRead(sensorH2);
      h3 = digitalRead(sensorH3);
      h4 = digitalRead(sensorH4);

      // Read IR proximity sensor
      obstacleDetected = digitalRead(proximitySensorPin) == HIGH;
    }

    void moveForward() {
      Serial.println("FORWARD");
      motorLeft.setSpeed(200);
      motorRight.setSpeed(200);
      motorLeft.run(FORWARD);
      motorRight.run(FORWARD);
    }

    void turnLeft() {
      Serial.println("LEFT TURN");
      motorLeft.setSpeed(150);
      motorRight.setSpeed(200);
      motorLeft.run(BACKWARD);
      motorRight.run(FORWARD);
      delay(500); // Adjust the delay value based on the required turn angle
      motorLeft.run(RELEASE);
      motorRight.run(RELEASE);
      logPath("L");
    }

    void turnRight() {
      Serial.println("RIGHT TURN");
      motorLeft.setSpeed(200);
      motorRight.setSpeed(150);
      motorLeft.run(FORWARD);
      motorRight.run(BACKWARD);
      delay(500); // Adjust the delay value based on the required turn angle
      motorLeft.run(RELEASE);
      motorRight.run(RELEASE);
      logPath("R");
    }

    void backTurn() {
      Serial.println("BACK TURN");
      motorLeft.setSpeed(200);
      motorRight.setSpeed(200);
      motorLeft.run(BACKWARD);
      motorRight.run(FORWARD);
      delay(1000); // Adjust this delay for a complete 180-degree turn
      motorLeft.run(RELEASE);
      motorRight.run(RELEASE);
      logPath("B");
    }

    void stopRobot() {
      motorLeft.run(RELEASE);
      motorRight.run(RELEASE);
    }

    void logPath(String move) {
      path += move;
    }

    void handleEnd() {
      stopRobot();
      Serial.println("Reached endpoint!");
      Serial.println(path);
    }

    void handleProximity() {
      if (obstacleDetected) {
        Serial.println("Obstacle detected!");
        backTurn(); // Treat obstacle like a dead-end and turn around
      }
    }

    void handlePath() {
      handleProximity(); // Check for obstacles
      
      // Straight dead-end
      if (v1 && !v2 && h1 && h2 && h3 && h4) {
        backTurn();
      }
      // Left or right turns
      else if ((h1 && h2) || (h2 && h3)) {
        if (h1) turnLeft();
        else turnRight();
      }
      // Cross
      else if (v1 && v2 && h1 && h2 && h3 && h4) {
        turnLeft(); // Default action for cross
      }
      // Default: Move forward or retrack
      else {
        moveForward();
      }
    }
};

MazeRobot robot;

void setup() {
  Serial.begin(9600);

  // Initialize sensors
  pinMode(sensorV1, INPUT);
  pinMode(sensorV2, INPUT);
  pinMode(sensorH1, INPUT);
  pinMode(sensorH2, INPUT);
  pinMode(sensorH3, INPUT);
  pinMode(sensorH4, INPUT);
  pinMode(proximitySensorPin, INPUT);

  // Stop the robot initially
  robot.stopRobot();
}

void loop() {
  robot.readSensors();

  // Detect end of maze
  if (robot.v1 && robot.v2 && robot.h1 && robot.h2 && robot.h3 && robot.h4) {
    robot.handleEnd();
    return;
  }

  // Handle the maze logic
  robot.handlePath();
}
