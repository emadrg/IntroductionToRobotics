//Declaring all the pins for the joystick
const int pinSW = 2; 
const int pinX = A0; 
const int pinY = A1; 

//Declaring the pins for the 7 segment display
const int segmentPins[] = {12, 10, 9, 8, 7, 6, 5, 4};
bool segmentPins_state[] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW }; //Array for keeping track of the segment's state of light, after pressing the joystick button
const int segSize = 8;

bool joyMoved = false;
int minThreshold = 400;
int maxThreshold = 600;
int xValue = 0;
int yValue = 0;
int activeSegment = 7; // We start on the dp segment 
bool buttonPressed = false;
bool segmentOn = false;
unsigned long lastBlinkMillis = 0;
unsigned long currentMillis = 0;
const unsigned long blinkInterval = 500;
bool lastSwState = LOW;
int buttonPressTime = 0;
bool isBlinking = false;  
const int debounceDelay = 50;

void setup() {
  //Setting up the pins and deciding if they're input or output
  pinMode(pinSW, INPUT_PULLUP);

  for (int i = 0; i < segSize; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], segmentPins_state[i]);
  }

  Serial.begin(9600);
}

void loop() {
  //Displaying the state of the segments 
  for (int i = 0; i < segSize; i++) {
    if (segmentPins_state[i] == LOW)
      digitalWrite(segmentPins[i], LOW);
    else if (segmentPins_state[i] == HIGH)
      digitalWrite(segmentPins[i], HIGH);
  }

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

// Deciding how to interpret the joystick moves, based on its values on the X and Y directions
  if (xValue < minThreshold && joyMoved == false) {
    activeSegment = nextSegment(activeSegment, 'L'); // L for left
    joyMoved = true;
  } else if (xValue > maxThreshold && joyMoved == false) {
    activeSegment = nextSegment(activeSegment, 'R'); // R for right
    joyMoved = true;
  }

  if (yValue < minThreshold && joyMoved == false) {
    activeSegment = nextSegment(activeSegment, 'U'); // U for up
    joyMoved = true;
  } else if (yValue > maxThreshold && joyMoved == false) {
    activeSegment = nextSegment(activeSegment, 'D'); // D for down
    joyMoved = true;
  }

// If the joystick doesn't move far enough to one specific direction, we can consider that it hasn't moved
  if (xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }

//Making the active segment nlink, regardless of its state in the segmentPins_state[] array
 blink(activeSegment, lastBlinkMillis, blinkInterval);
 
 //Reading the input from the joystick's button
  bool swState = digitalRead(pinSW);
 //Deciding if the button was pressed. I used debounce for more accuracy and to avoid unwanted accidental activity of the button
  if (swState != lastSwState) {
    if (swState == LOW) {
       if ((millis() - buttonPressTime) > debounceDelay) {
        buttonPressed = !buttonPressed;
        buttonPressTime = millis();  // Update the button press time
        segmentPins_state[activeSegment] = !segmentPins_state[activeSegment]; // If the button is pressed, we update the state array
        digitalWrite(segmentPins[activeSegment], segmentPins_state[activeSegment]); // We display the change
      }
    }

    lastSwState = swState; 
  }

 
}

// Blink function, to make the active segment blink when the joystick is on it
void blink(int segment, unsigned long &lastBlinkMillis, unsigned long blinkInterval) {
 
  currentMillis = millis();

  if (currentMillis - lastBlinkMillis >= blinkInterval) {
    lastBlinkMillis = currentMillis;

    int ledState = digitalRead(segmentPins[segment]);

    digitalWrite(segmentPins[segment], !ledState); // Invert the LED state to make it blink4
      delay(100);
  }
}

// Function to determine the next segment, based on the current segment and the movements of the joystick
int nextSegment(int segment, char direction) {
if (segment == 0) {
   if (direction == 'U') return 0;
   if (direction == 'D') return 6;
   if (direction == 'L') return 5;
   if (direction == 'R') return 1;
 }
 if (segment == 1) {
   if (direction == 'U') return 0;
   if (direction == 'D') return 2;
   if (direction == 'L') return 5;
   if (direction == 'R') return 1;
 }
 if (segment == 2) {
   if (direction == 'U') return 1;
   if (direction == 'D') return 3;
   if (direction == 'L') return 4;
   if (direction == 'R') return 7;
 }
 if (segment == 3) {
   if (direction == 'U') return 6;
   if (direction == 'D') return 3;
   if (direction == 'L') return 4;
   if (direction == 'R') return 2;
 }
 if (segment == 4) {
   if (direction == 'U') return 5;
   if (direction == 'D') return 3;
   if (direction == 'L') return 4;
   if (direction == 'R') return 2;
 }
 if (segment == 5) {
   if (direction == 'U') return 0;
   if (direction == 'D') return 4;
   if (direction == 'L') return 5;
   if (direction == 'R') return 1;
 }
 if (segment == 6) {
   if (direction == 'U') return 0;
   if (direction == 'D') return 3;
   if (direction == 'L') return 6;
   if (direction == 'R') return 6;
 }
 if (segment == 7) {
   if (direction == 'U') return 7;
   if (direction == 'D') return 7;
   if (direction == 'L') return 2;
   if (direction == 'R') return 7;
 }

}
