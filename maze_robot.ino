#include <AFMotor.h>

// Sensor and Motor Pins
const int sensorV1 = A0; // Vertical bottom
const int sensorV2 = A1; // Vertical top
const int sensorH1 = A2; // Leftmost horizontal
const int sensorH2 = A3; // Inner left horizontal
const int sensorH3 = A4; // Inner right horizontal
const int sensorH4 = A5; // Rightmost horizontal
const int proximitySensor = 8; // Proximity sensor for dead-ends

// Motor setup
AF_DCMotor motorLeft(3);  // Left motor (M3)
AF_DCMotor motorRight(4); // Right motor (M4)

// Robot class
class MazeRobot {
  public:
    int v1, v2, h1, h2, h3, h4, proximity; // Sensor readings
    String path;                           // Path log

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
      proximity = digitalRead(proximitySensor);
    }

    void moveForward() {
      motorLeft.setSpeed(200);
      motorRight.setSpeed(200);
      motorLeft.run(FORWARD);
      motorRight.run(FORWARD);
      logPath("F");
    }

    void turnLeft() {
      motorLeft.setSpeed(150);
      motorRight.setSpeed(200);
      motorLeft.run(BACKWARD);
      motorRight.run(FORWARD);
      delay(400);
      logPath("L");
    }

    void turnRight() {
      motorLeft.setSpeed(200);
      motorRight.setSpeed(150);
      motorLeft.run(FORWARD);
      motorRight.run(BACKWARD);
      delay(400);
      logPath("R");
    }

    void turnBack() {
      // Back turn (180-degree turn)
      motorLeft.setSpeed(200);
      motorRight.setSpeed(200);
      motorLeft.run(BACKWARD);
      motorRight.run(BACKWARD);
      delay(1000); // Adjust the time to get a full 180-degree turn
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

    void handleDeadEnd() {
      if (proximity) {
        stopRobot();
        delay(1000);
        turnBack(); // Back turn for dead-end handling
        logPath("B");
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
    }

    void handleIntersection() {
      // T-left
      if (v1 && v2 && !h1 && !h2 && !h3 && h4) {
        turnRight();
      }
      // T-right
      else if (v1 && v2 && h1 && !h2 && !h3 && !h4) {
        turnLeft();
      }
      // Cross intersection
      else if (h1 && h2 && h3 && h4 && v1 && v2) {
        turnLeft(); // Default action for cross
      }
    }

    void handlePath() {
      // Straight deadend
      if (v1 && !v2 && h1 && h2 && h3 && h4) {
        moveForward();
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
      int changes;
      do {
        changes = 0;
        // Remove redundant pairs
        if (optimizedPath.indexOf("LR") >= 0) {
          optimizedPath.replace("LR", "");
          changes++;
        }
        if (optimizedPath.indexOf("RL") >= 0) {
          optimizedPath.replace("RL", "");
          changes++;
        }
      } while (changes > 0);

      Serial.println("Optimized Path: " + optimizedPath);
      path = optimizedPath;
    }

    void executeOptimizedPath() {
      for (int i = 0; i < path.length(); i++) {
        char move = path[i];
        if (move == 'F') moveForward();
        if (move == 'L') turnLeft();
        if (move == 'R') turnRight();
        if (move == 'B') turnBack(); // Execute back turn
        delay(500); // Adjust delay for robot speed
      }
      stopRobot();
    }

    void handleEnd() {
      stopRobot();
      Serial.println("Reached endpoint!");
      optimizePath();
      executeOptimizedPath();
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
  robot.handleDeadEnd(); // Check for dead-end
  robot.handlePath();
}
