const int button_floor[3] = {2, 3, 4};
const int led_floor[3] = {5, 6, 7};
const int operational_led = 8;
const int buzzer = 9;

int current_floor = 0;
int destination_floor = 1;
bool isMoving = false;
bool doorsAreClosed = true;
bool elevatorIsStationary = true;
unsigned long lastButtonPressTime = 0;
const unsigned long debounceDelay = 50; 
unsigned long previousMillis = 0;
const long floorChangeInterval = 2000; //time to wait between floor changes
const long operationalLEDInterval = 500; //interval for operational LED blinking (millis)

int buzzer_freq_moving = 500;// frequency for the buzzer when the elevator is moving
int buzzer_freq_beep = 1500;//ffrequency for the buzzer when the elevator reached the destination floor
int buzz_beep_delay_time = 200; //delay time for when the buzzer is beeping
bool operational_led_state = LOW;
unsigned long previousOperationalMillis = 0;

void setup() {

  for (int i = 0; i < 3; i++) {

    pinMode(led_floor[i], OUTPUT);
    pinMode(button_floor[i], INPUT_PULLUP);

  }

  pinMode(operational_led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  updateFloorLED(current_floor);

  Serial.begin(9600);
}

void loop() {

  unsigned long currentMillis = millis();

  for (int i = 0; i < 3; i++) {
    if (digitalRead(button_floor[i]) == LOW) {
      if ((currentMillis - lastButtonPressTime) > debounceDelay) {
        lastButtonPressTime = currentMillis;
        if (i + 1 != current_floor && doorsAreClosed) {
          destination_floor = i + 1;
          if (!isMoving) {
            moveElevator();
          }
        }
      }
    }
  }

  if (isMoving) {
    if (current_floor == destination_floor) {
      stopElevator();
    } else {
      buzz(buzzer_freq_moving);
      updateFloorLED(current_floor);
      elevatorIsStationary = false;
      blinkOperationalLED();
    }
  } else {
    digitalWrite(operational_led, HIGH);
    if (elevatorIsStationary) {
      updateFloorLED(current_floor);
    }
  }
}



//additional functions

void moveElevator() {
  isMoving = true;
  doorsAreClosed = false;

  digitalWrite(operational_led, HIGH);

  digitalWrite(led_floor[current_floor - 1], LOW);

  unsigned long previousMillis = millis();

  while (current_floor != destination_floor) {
    if (current_floor < destination_floor && (millis() - previousMillis) >= floorChangeInterval) {
      previousMillis = millis();
      current_floor++;
    } else if (current_floor > destination_floor && (millis() - previousMillis) >= floorChangeInterval) {
      previousMillis = millis();
      current_floor--;
    }
    updateFloorLED(current_floor);
    buzz(buzzer_freq_moving);
    blinkOperationalLED();
  }

  beep(buzzer_freq_beep);
  stopElevator();

}

//Stopping the elevator when it reached the desired floor
void stopElevator() {
  digitalWrite(operational_led, HIGH); // making the operational led stationary on HIGH
  isMoving = false;
  doorsAreClosed = true;
  destination_floor = current_floor;
  elevatorIsStationary = true;
  digitalWrite(led_floor[current_floor - 1], HIGH);
}

//This function makes the corresponding LED light up when the elevator is on that floor
void updateFloorLED(int floor) {
  for (int i = 0; i < 3; i++) {
    if (i == floor - 1) {
      digitalWrite(led_floor[i], HIGH);
    } else  {
      digitalWrite(led_floor[i], LOW);
    }
  }
}

void buzz(int frequency) {
  tone(buzzer, frequency);
}

void beep(int frequency) {
  buzz(frequency);
  delay(buzz_beep_delay_time);
  noTone(buzzer);
  delay(buzz_beep_delay_time);
  buzz(buzzer_freq_beep);
  delay(buzz_beep_delay_time);
  noTone(buzzer);
}

//Making the operational state led blink when the elevator is in movement
void blinkOperationalLED() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousOperationalMillis >= operationalLEDInterval) {
    previousOperationalMillis = currentMillis;
    operational_led_state = (operational_led_state == LOW) ? HIGH : LOW;
    digitalWrite(operational_led, operational_led_state);
  }
}