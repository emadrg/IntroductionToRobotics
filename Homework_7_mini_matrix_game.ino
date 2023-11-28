#include "LedControl.h"

const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
byte matrixBrightness = 2;
int xValue, yValue;
int minThreshold = 400;
int maxThreshold = 600;
bool joyMoved = false;
int row = 1;
int col = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
bool buttonState = HIGH;
bool lastButtonState = HIGH;
int playerRow = 3;
int playerCol = 3;
const int numObstacles = 3;

//I defined this matrix in order to go through the obstacles more easily, and also be able to add more in case I need to do so, without having to modify the rest of the code
bool obstacles[numObstacles][matrixSize][matrixSize] = {
  // Obstacle 1
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },

  // Obstacle 2
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },

  // Obstacle 3
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }
};

// Example of accessing obstacle data
bool (*currentObstacle)[matrixSize] = obstacles[0];


void setup() {
  row = 7;
  col = 3; // the initial position of the shooter(player)
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
  pinMode(pinSW, INPUT_PULLUP);
  // Set the initial obstacle;
  currentObstacle = obstacles[0];
  displayObstacle();
  Serial.begin(9600);
  Serial.println("Program started"); //Serial print used for debugging. I'm going to leave it here since it's useful to see how the code runs
}

void loop() {
  blinkLedPlayer(row, col, 100);
  decideJoyDirection(joyMoved);
  handleLaserShoot();
}


//Function for deciding how to interpret the moves of the joystick. I 'recycled' this part of the code from one of my previous projects
void decideJoyDirection(bool &joyMoved) { 
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if (xValue < minThreshold && !joyMoved) {  // left
    moveSegment('L');
    joyMoved = true;
  } else if (xValue > maxThreshold && !joyMoved) {  // right
    moveSegment('R');
    joyMoved = true;
  }

  if (yValue < minThreshold && !joyMoved) {  // up
    moveSegment('U');
    joyMoved = true;
  } else if (yValue > maxThreshold && !joyMoved) {  // down
    moveSegment('D');
    joyMoved = true;
  }

  if (xValue > minThreshold && xValue < maxThreshold && yValue > minThreshold && yValue < maxThreshold) {
    joyMoved = false;  // reset joyMoved when joystick returns to center
  }
}

//Moving the player according to the moves of the joysick
void moveSegment(char direction) {
  lc.setLed(0, row, col, false);

  if (direction == 'U' && row > 0) {
    row--;
  } else if (direction == 'D' && row < matrixSize - 1) { // Keeping in mind that the player can't move out of the matrix
    row++;
  } else if (direction == 'L' && col > 0) {
    col--;
  } else if (direction == 'R' && col < matrixSize - 1) {
    col++;
  }

  lc.setLed(0, row, col, true);
}


// Blinking the player LED in order to differenciate it from the obstacles/laser
void blinkLedPlayer(int row, int col, int interval) {
  static unsigned long previousMillis = 0;
  static bool ledState = false;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;  // Toggle the LED state

    lc.setLed(0, row, col, ledState);
  }
}

//Logic for deciding if a laser is shot, depending on the activity of the joystick button
void handleLaserShoot() {
  int reading = digitalRead(pinSW);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        shootLaser(row, col);
      }
    }
  }

  lastButtonState = reading;
}

//Visual representation of the laser shooting. Here the obstacles are destroyed when shot
void shootLaser(int row, int col) {
  for (int i = 1; i < 8; i++) {
    if (currentObstacle[row - i][col] == 1) {
      currentObstacle[row - i][col] = 0;
      lc.setLed(0, row - i, col, false);

      // Check if the entire obstacle is destroyed
      bool obstacleDestroyed = true;
      for (int r = 0; r < matrixSize; r++) {
        for (int c = 0; c < matrixSize; c++) {
          if (currentObstacle[r][c] == 1) {
            obstacleDestroyed = false;
            break;
          }
        }
        if (!obstacleDestroyed) {
          break;
        }
      }

      // If the obstacle is fully destroyed, switch to the next one
      if (obstacleDestroyed) {
        switchToNextObstacle();
      }

      return;
    }

    lc.setLed(0, row - i, col, true);
    delay(20);
    lc.setLed(0, row - i, col, false);
  }
}

//Function for displaying the obstacles one by one, on the matrix
void displayObstacle() {
  lc.clearDisplay(0);

  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      if (currentObstacle[i][j] == 1) {
        lc.setLed(0, i, j, true);
      }
    }
  }
}

//Function for going through all the obstacles in the array. When a new obstacle is added, I don't need to modify this function since I used an array
void switchToNextObstacle() {
  static int currentObstacleIndex = 0;
  currentObstacleIndex = (currentObstacleIndex + 1) % numObstacles;
  currentObstacle = obstacles[currentObstacleIndex];
  Serial.print("Switched to obstacle index: ");
  Serial.println(currentObstacleIndex);
  // Clear the display and show the new obstacle
  lc.clearDisplay(0);
  delay(100);
  displayObstacle();
}



