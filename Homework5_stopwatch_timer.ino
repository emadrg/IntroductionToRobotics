// Define connections to the shift register
const int latchPin = 11;
const int clockPin = 10;
const int dataPin = 12; 
// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;
// Define the pins for the buttons
const int btn_start_pause = 9;
const int btn_reset = 8;
const int btn_save_lap = 2;

// Define the number of laps that should be memorized in order to avooid "magic numbers"
const int no_laps = 4;

// Store the digits in an array for easy access
int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4; // Number of digits in the display
// Define the number of unique encodings (0-9, A-F for hexadecimal)
const int encodingsNumber = 16;
// Define byte encodings for the hexadecimal characters 0-F
byte byteEncodings[encodingsNumber] = {
  // A B C D E F G DP
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110 // F
};

// Variables for controlling the display update timing
unsigned long lastIncrement = 0;
unsigned long delayCount = 50; // Delay between updates (milliseconds)
unsigned long number = 0; // The number being displayed

// Variables for lap functionality
bool timerRunning = false;
bool pauseMode = false;
unsigned long lapTimes[4]; // Array to store lap times
int lapIndex = 0; // Index to keep track of the current lap
volatile bool saveLapInterrupt = false;
volatile bool pauseInterrupt = false;

void setup() {
  // Initialize the pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(btn_start_pause, INPUT);
  pinMode(btn_reset, INPUT);
  pinMode(btn_save_lap, INPUT);

  // Initialize digit control pins and set them to LOW (off)
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  
  // Set up interrupts for save lap and pause buttons
  attachInterrupt(digitalPinToInterrupt(btn_save_lap), saveLapISR, RISING);
  attachInterrupt(digitalPinToInterrupt(btn_start_pause), pauseISR, RISING);
 
  Serial.begin(9600);
   
}

void saveLapISR() {
  saveLapInterrupt = true;
}

void pauseISR() {
  pauseInterrupt = true;
}

void loop() {

  // Check if the start/pause button is pressed
  
  if (digitalRead(btn_start_pause) == HIGH) {
    if (!timerRunning) {
      // Start the timer
      timerRunning = true;
      pauseMode = false;
    } else {
      // Pause the timer
      timerRunning = false;
      pauseMode = true;
    }
    delay(500);
  }

  // Check if the reset button is pressed
  if (digitalRead(btn_reset) == HIGH) {
    if (!timerRunning) {
      // Reset the timer and lap data
      number = 0;
      lapIndex = 0;
      for (int i = 0; i < no_laps; i++) {
        lapTimes[i] = 0;
      }
    } else if (!pauseMode) {
      // Reset to 000.0 only in PauseMode
      number = 0;
    }
    delay(500);
  }

  // Check for save lap interrupt
  if (saveLapInterrupt && !pauseMode) {
    // Save lap time if not in PauseMode
    lapTimes[lapIndex] = number;
    lapIndex = (lapIndex + 1) % no_laps; // Cycle through laps
    saveLapInterrupt = false;
  }

  // Check for pause interrupt
  if (pauseInterrupt) {
    if (timerRunning) {
      // Pause the timer
      timerRunning = false;
      pauseMode = true;
    } else if (pauseMode) {
      // Resume the timer
      timerRunning = true;
      pauseMode = false;
    }
    pauseInterrupt = false;
  }

  // Display logic
  if (timerRunning && !pauseMode) {
    // Increment the number if the timer is running
    if (millis() - lastIncrement > delayCount) {
      number++;
      number %= 10000; // Wrap around after 9999
      lastIncrement = millis();
    }
  }

  // Display lap time when in PauseMode and cycling through laps
  if (pauseMode && saveLapInterrupt) {
    number = lapTimes[lapIndex];
    saveLapInterrupt = false;
  }

  // Display the current number on the 7-segment display using multiplexing
  writeNumber(number);
}

void writeReg(int digit) {
  // Prepare to shift data by setting the latch pin low
  digitalWrite(latchPin, LOW);
  // Shift out the byte representing the current digit to the shift register
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  // Latch the data onto the output pins by setting the latch pin high
  digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  // Turn off all digit control pins to avoid ghosting
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // Turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  // Initialize necessary variables for tracking the current number and digit position
  int currentNumber = number;
  int displayDigit = 3; // Start with the least significant digit
  int lastDigit = 0;
  // Loop through each digit of the current number
  while (currentNumber != 0) {
    // Extract the last digit of the current number
    lastDigit = currentNumber % 10;
    // Activate the current digit on the display
    activateDisplay(displayDigit);
    // Output the byte encoding for the last digit to the display
    writeReg(byteEncodings[lastDigit]);
    // Implement a delay if needed for multiplexing visibility
    delay(0); // A delay can be increased to visualize multiplexing
    // Move to the next digit
    displayDigit--;
    // Update 'currentNumber' by removing the last digit
    currentNumber /= 10;
    // Clear the display to prevent ghosting between digit activations
    writeReg(B00000000); // Clear the register to avoid ghosting
  }
}