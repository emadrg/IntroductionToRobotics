#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

// LCD declarations
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const int pwm_pin = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int storedBrightness = 0;
int brightnessValue = 0;

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
int currentObstacleIndex = 0;

bool gameRunning = false;
bool difficultySelected = false;

const int MENU_ITEMS = 3;  // Number of items in the menu
int currentMenuItem = 0;   // Currently selected menu item

char currentDirection = 'N';
bool increaseBrightness = false;

int ledsShotObstacle = 0;


bool currentMatrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

// I defined this matrix in order to go through the obstacles more easily,
// and also be able to add more in case I need to do so, without having to modify the rest of the code
bool obstacles[numObstacles][matrixSize][matrixSize] = {
  // Obstacle 1
  { { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 1, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 } },

  // Obstacle 2
  { { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 1, 0, 0 },
    { 0, 0, 1, 0, 0, 1, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 } },

  // Obstacle 3
  { { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 1, 0, 0 },
    { 0, 0, 1, 0, 0, 1, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 1, 0, 0, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 } }
};

unsigned long obstacleMoveInterval = 500;  // Adjust the interval as needed
unsigned long lastObstacleMoveTime = 0;

int difficulty = 2;  // Default difficulty level

int ledsShot = 0;

String main_menu[3] = { "Start game >", " < Settings > ", " <About" };
String settings_menu[2] = { "LCD Brightness >", "< Matrix brigtness" };
String difficulty_menu[3] = {"1>", "<2>", "<3"};



void setup() {
  // Initialize LCD and set up the number of columns and rows:
  lcd.begin(16, 2);

  // Set initial values for LCD and matrix brightness from EEPROM
  matrixBrightness = EEPROM.read(1);
  lc.setIntensity(0, matrixBrightness);

  // Set the initial LCD brightness to maximum
  analogWrite(pwm_pin, 255);

  // Initialize pins and initial positions
  row = 7;  // Set the initial row to a valid starting position
  col = 3;  // Set the initial col to a valid starting position; I chose the player to be on the last row, middle column for easier movement
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
  pinMode(pinSW, INPUT_PULLUP);

  // Set the initial obstacle
  reset_current_obstacle(0);
  displayObstacle();

  // Display the player at the initial position
  lc.setLed(0, row, col, true);

  // Display the welcome message
  displayIntroMessage();

  // Print a message to the Serial monitor for debugging
  Serial.begin(9600);
  Serial.println("Program started");
}




void loop() {
  lcd.cursor();
  //If the game hasn't started yet, you can choose the difficulty
  if (!gameRunning) {
    if (!difficultySelected) {
      //displayMainMenu();
      
      setObstacleMoveInterval();
      difficultySelected = true;
    } else {  //If the game has started, the main menu is shown; come back here for further modifications (lives, game status etc)
      handleMainMenu();
      //handleMainMenu();
    }
  } else {
    // Game logic here
    handleGame();
  }
}


//Function for deciding how to interpret the moves of the joystick. I 'recycled' this part of the code from one of my previous projects
bool decideJoyDirection() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if (xValue < minThreshold && !joyMoved) {  // left
    currentDirection = 'L';
    joyMoved = true;
  } else if (xValue > maxThreshold && !joyMoved) {  // right
    currentDirection = 'R';
    joyMoved = true;
  } else if (yValue < minThreshold && !joyMoved) {  // up
    currentDirection = 'U';
    joyMoved = true;
  } else if (yValue > maxThreshold && !joyMoved) {  // down
    currentDirection = 'D';
    joyMoved = true;
  }

  if (xValue > minThreshold && xValue < maxThreshold && yValue > minThreshold && yValue < maxThreshold) {
    joyMoved = false;  // reset joyMoved when joystick returns to center
  }
}

bool scrollMenu(int &index, int size) {
  decideJoyDirection();
  Serial.print(currentDirection);
  Serial.print('-');
  Serial.println(joyMoved);

  if (!joyMoved) {
    return false;
  }

  switch (currentDirection) {
    case 'L':
      index = (index - 1);
      if (index < 0) {
        index = 0;
      }
      break;
    case 'R':
      index = (index + 1);
      if (index >= size) {
        index = size - 1;
      }
      break;
    // Do nothing for other directions
    default:
      return false;
  }
  // Reset currentDirection to avoid continuous scrolling
  currentDirection = 'N';

  lcd.clear();  // Clear the display before printing the new content
  lcd.setCursor(0, 0);

  return true;
}


  int index_main = 0;
void handleMainMenu() {
  // 3 is the size of the main menu
  if (scrollMenu(index_main, 3)) {
    lcd.print(main_menu[index_main]);
  }

  if (buttonPressed()) {
    Serial.print("se apasa buton la meniul principal");
    switch (index_main) {
      case 0:
        startGame();
        break;
      case 1:
        settings();
        break;
      case 2:
        about();
        break;
    }
  }
}



bool scrollMenu_difficulty(int &index, int size) {
  decideJoyDirection();
  Serial.print(currentDirection);
  Serial.print('-');
  Serial.println(joyMoved);

  if (!joyMoved) {
    Serial.println("A INTRAT PE IF");
    return false;
  }

  switch (currentDirection) {
    case 'L':
      index = (index - 1);
      if (index < 0) {
        index = 0;
      }
      break;
    case 'R':
      index = (index + 1);
      if (index >= size) {
        index = size - 1;
      }
      break;

    // Do nothing for other directions
    default:
      return false;
  }
  // Reset currentDirection to avoid continuous scrolling
  currentDirection = 'N';

  lcd.clear();  // Clear the display before printing the new content
  lcd.setCursor(0, 0);

  return true;
}



void startGame() {

  lcd.clear();
  lcd.setCursor(0, 0);
 

  // In this function, you can also select the difficulty
  //Only after the difficulty is selected, should the game start
  if (scrollMenu(currentMenuItem, 3)) {
    lcd.setCursor(0, 1);
    lcd.print(difficulty_menu[currentMenuItem]);
  }

  if (!buttonPressed()) {
      Serial.print("a intrat pe fct");
    switch (currentMenuItem) {
      case 0:
        setDifficulty(1);
        break;
      case 1:
        setDifficulty(2);
        break;
      case 2:
        setDifficulty(3);
        break;
    }
  }
 
}


void settings() {
  lcd.clear();
  lcd.setCursor(0, 0);
  //lcd.print("Settings Menu...");

  handleSettingsMenu();

  delay(2000);  // Display the message for 2 seconds
}

void about() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("About Menu... ala bala portocala bla bla unu doi trei");
  lcd.scrollDisplayRight();
  delay(2000);  // Display the message for 2 seconds
}


int index_settings = 0;
void handleSettingsMenu() {

  // 2 is the size of the settings menu
  if (scrollMenu(index_settings, 2)) {
    lcd.print(settings_menu[index_settings]);
  }

  if (buttonPressed()) {
    switch (index_settings) {
      case 0:
        LCDBrightness();
        break;
      case 1:
       // matrix_brightness();
        break;
      default: 
        break;
    }
  }
}

void LCDBrightness() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Adjust LCD Brightness");

  bool adjustingBrightness = true;

  // Allow adjustments using joystick movements
  while (adjustingBrightness) {
    decideJoyDirection();
    delay(100); // Adjust the delay based on responsiveness requirements
  }

  // Save the new brightness value to EEPROM
  EEPROM.update(1, matrixBrightness);

  // Display a confirmation message or return to the settings menu
  lcd.clear();
  lcd.print("Brightness adjusted");
  delay(500); // Display the message for 2 seconds
}

void handleAboutMenu() {
  if (buttonPressed()) {
    lcd.print("about display");
  }
}

//Function for deciding if the button was pressed
bool buttonPressed() {
  int reading = digitalRead(pinSW);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        return true;
      }
    }
  }

  lastButtonState = reading;
  return false;
}


void setDifficulty(int selectedDifficulty) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Difficulty: " + String(selectedDifficulty));
  
  // Set the difficulty variable to the selected difficulty
  difficulty = selectedDifficulty;
  gameRunning = true;
  //delay(2000);  // Display the selected difficulty for 2 seconds
}


void moveSegment(char direction) {

  Serial.print("Moving in direction: ");  //Serial print for easier debugging
  Serial.println(direction);

  lc.setLed(0, row, col, false);

  if (direction == 'U' && row > 0) {
    row--;
  } else if (direction == 'D' && row < matrixSize - 1) {  // Keeping in mind that the player can't move out of the matrix
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
        Serial.println("Button Pressed");
        shootLaser(row, col);

        // Increment the number of LEDs shot in the current obstacle
        ledsShotObstacle++;
      }
    }
  }

  lastButtonState = reading;
}



//Visual representation of the laser shooting. Here the obstacles are destroyed when shot, one LED at a time
void shootLaser(int row, int col) {
  for (int i = 1; i < 8; i++) {
    if (currentMatrix[row - i][col] == 1) {
      currentMatrix[row - i][col] = 0;
      lc.setLed(0, row - i, col, false);

      // Increment the number of LEDs shot, in order to display it during the game
      ledsShot++;

      // Check if the entire obstacle is destroyed
      bool obstacleDestroyed = true;
      for (int r = 0; r < matrixSize; r++) {
        for (int c = 0; c < matrixSize; c++) {
          if (currentMatrix[r][c] == 1) {
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
  // Iterate through the currentMatrix and turn on LEDs
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      if (currentMatrix[i][j] == 1) {
        lc.setLed(0, i, j, true); // Turn on the LED without clearing the display
      }
    }
  }
}


//Function for going through all the obstacles in the array. When a new obstacle is added, I don't need to modify this function since I used an array.

void switchToNextObstacle() {
  // Check if no LEDs were shot in the current obstacle
  if (ledsShotObstacle == 0) {
    gameRunning = false;
    displayEndMessage();
    return;
  }

  // Reset the number of LEDs shot in the current obstacle
  ledsShotObstacle = 0;

  currentObstacleIndex = (currentObstacleIndex + 1) % numObstacles;

  reset_current_obstacle(currentObstacleIndex);
  Serial.print("Switched to obstacle index: ");
  Serial.println(currentObstacleIndex);

  // Clear the display and show the new obstacle
  lc.clearDisplay(0);
  delay(100);
  displayObstacle();
}






//Function where the currentMatrix takes the value of each obstacle's matrix.
//Here the currentMatrix takes the values of each the matrix of each obstacle. I did this so that when the obstacles are destroyes, the modifications happen over this
//currentMatrix, not the ones where the obstacles are defined. If I hadn't used this, the declared matrixes would be destroyed.
void reset_current_obstacle(int index) {
  for (int i = 0; i < matrixSize; i++)
    for (int j = 0; j < matrixSize; j++) {
      currentMatrix[i][j] = obstacles[index][i][j];
    }
}

//This function allows me to control the contrast of my LCD using a potentiometer.
void setLCDcontrast() {
  analogWrite(pwm_pin, 255);
  // read the potentiometer on A0:
  int sensorReading = analogRead(A0);
  // map the result to 200 - 1000:
  int delayTime = map(sensorReading, 0, 1023, 200, 1000);
  delay(delayTime);
}

// Function to remove the leds from the obstacles one by one. This is used to add some difficulty to the game.
//The idea is to shoot as many LEDs from the obstacle as possible, but you have to be fast, since they dissapear one by one
//TO DO: deoending on the difficulty, the leds should dissapear faster
void removeObstacleLeds() {
  // Find the first LED in the obstacle that is currently ON and turn it OFF
  for (int i = matrixSize - 1; i >= 0; i--) {
    for (int j = matrixSize - 1; j >= 0; j--) {
      if (currentMatrix[i][j] == 1) {
        currentMatrix[i][j] = 0;  // Turn off the current LED
        displayObstacle();        // Display the updated obstacle
        return;                   // Exit the function after turning off one LED
      }
    }
  }
}


//This is the general function for playing the game. The obstacle LEDS are dissapearing one by one, at a speed depending on the chosen difficulty.
//
void handleGame() {
  unsigned long currentTime = millis();
  if (currentTime - lastObstacleMoveTime >= obstacleMoveInterval) {
    removeObstacleLeds();
    lastObstacleMoveTime = currentTime;
  }

  // Check if the current obstacle is fully cleared
  bool currentObstacleCleared = true;
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      if (currentMatrix[i][j] == 1) {
        currentObstacleCleared = false;
        break;
      }
    }
    if (!currentObstacleCleared) {
      break;
    }
  }

  // If the current obstacle is cleared, switch to the next one
  if (currentObstacleCleared) {
    switchToNextObstacle();
  }

  // Update player position and check shooting logic
  decideJoyDirection();
  if (joyMoved) {
    moveSegment(currentDirection);
    currentDirection = 'N';
  }
  handleLaserShoot();

  // Display the number of LEDs shot on the LCD
  lcd.setCursor(0, 2);
  lcd.print("LEDs Shot: " + String(ledsShot));
}



//This function displayes an intro message at the beginning of the game
void displayIntroMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome!");
  delay(2000);  // Display the message for 2 seconds
  
}

//Message to display when the game is over (the player lost)
void displayEndMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over! Press button");
  lcd.setCursor(0, 1);
  lcd.print("to return to main menu");
  waitForButtonPress();  // Wait for the button press before returning to the main menu
}


void waitForButtonPress() {
  lcd.noDisplay();  // Turn off the display
  while (digitalRead(pinSW) == HIGH) {
    // Wait for the button press
  }
  while (digitalRead(pinSW) == LOW) {
    // Wait for the button release
  }
  lcd.display();  // Turn on the display
  delay(500);     // Debounce delay
}


void setObstacleMoveInterval() {
  switch (difficulty) {
    case 1:
      obstacleMoveInterval = 1000;  // Easy
      break;
    case 2:
      obstacleMoveInterval = 800;  // Medium
      break;
    case 3:
      obstacleMoveInterval = 500;  // Hard
      break;
  }
}
