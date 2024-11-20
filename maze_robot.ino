#include <AFMotor.h>

// Sensor and Motor Pins
const int sensorV1 = A0; // Vertical bottom
const int sensorV2 = A1; // Vertical top
const int sensorH1 = A2; // Leftmost horizontal
const int sensorH2 = A3; // Inner left horizontal
const int sensorH3 = A4; // Inner right horizontal
const int sensorH4 = A5; // Rightmost horizontal

// Motor setup
AF_DCMotor motorLeft(3);  // Left motor (M3)
AF_DCMotor motorRight(4); // Right motor (M4)

// Robot class
class MazeRobot {
  public:
    int v1, v2, h1, h2, h3, h4; // Sensor readings
    String path;                // Path log

    MazeRobot() {
      path = "";
    }

    void readSensors() {
      v1 = analogRead(sensorV1) > 500;
      v2 = analogRead(sensorV2) > 500;
      h1 = analogRead(sensorH1) > 500;
      h2 = analogRead(sensorH2) > 500;
      h3 = analogRead(sensorH3) > 500;
      h4 = analogRead(sensorH4) > 500;
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
    
      // Delay for turning left
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
    
      // Delay for turning right
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
    
      // Delay for a 180-degree back turn
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

    void retrack() {
      // If a horizontal sensor activates, correct position
      if (h1 || h2 || h3 || h4) {
        moveForward();
      }
    }

    void handleEnd() {
      stopRobot();
      Serial.println("Reached endpoint!");
      optimizePath();
    }

    void handleIntersection() {
      // Cross intersection logic
      if (h1 && h2 && h3 && h4 && v1 && v2) {
        turnLeft(); // Default action for cross
      }
    }

    void handleTurn() {
      // Left turn
      if (h1 && !h2 && !h3 && !h4 && !v2 && v1) {
        turnLeft();
      }
      // Right turn
      else if (!h1 && !h2 && !h3 && h4 && !v2 && v1) {
        turnRight();
      }
      // T-left
      else if (v1 && v2 && !h1 && !h2 && !h3 && h4) {
        turnRight();
      }
      // T-right
      else if (v1 && v2 && h1 && !h2 && !h3 && !h4) {
        turnLeft();
      }
    }

    void handlePath() {
      // Straight deadend
      if (v1 && !v2 && h1 && h2 && h3 && h4) {
        backTurn();
      }
      // T-shape
      else if (v1 && !v2 && h1 && h4 && !h2 && !h3) {
        handleTurn();
      }
      // Cross
      else if (v1 && v2 && h1 && h2 && h3 && h4) {
        handleIntersection();
      }
      // Default: Move forward or retrack
      else {
        retrack();
      }
    }

    void optimizePath() {
      String optimizedPath = path;
      optimizedPath.replace("LBL", "F");
      optimizedPath.replace("LBR", "B");
      optimizedPath.replace("RBL", "B");
      optimizedPath.replace("LBF", "R");
      optimizedPath.replace("RBR", "F");
      optimizedPath.replace("RBF", "L");
      optimizedPath.replace("FBL", "R");
      optimizedPath.replace("FBR", "L");
      optimizedPath.replace("FBF", "B");
      Serial.println("Optimized Path: " + optimizedPath);
    }
};

MazeRobot robot;

void setup() {
  Serial.begin(9600);
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
