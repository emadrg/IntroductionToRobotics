#include <EEPROM.h>

//Pins for the ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 11;


int photocellPin = 0;
int photocellValue = 0;

int distance = 0;

int sampling_interval = 10;
int min_threshold_ultrasonic = 2;
int min_threshold_LDR = 200;

const int RGB_red = 2;
const int RGB_blue = 3;
const int RGB_green = 4;

int index_ultrasonic = 0;
int index_LDR = 0;

int duration = 0;

int automatic_mode = 0;

int eeprom_index = 0;
int printing = 0;

const int max_readings = 10;
int ultrasonic_readings[max_readings] = { 0 };
int LDR_readings[max_readings] = { 0 };

void setup() {

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(RGB_red, OUTPUT);
  pinMode(RGB_green, OUTPUT);
  pinMode(RGB_blue, OUTPUT);

  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

  print_main_menu();
}

void loop() {

  sensors(photocellValue, distance, sampling_interval, ultrasonic_readings, LDR_readings, automatic_mode, index_ultrasonic, index_LDR);

  int choice = readNumber(eeprom_index);

  switch (choice) {
    case 1:
      submenuSensorSettings();
      break;
    case 2:
      submenuResetLoggerData();
      break;
    case 3:
      submenuSystemStatus();
      break;
    case 4:
      submenuRGBLEDControl();
      break;
    default:
      Serial.println("Invalid choice. Please enter a number between 1 and 4.");
  }
}

void submenuSensorSettings() {
  int choice;
  do {
    printSubMenuSensorSettings();
    choice = readNumber(eeprom_index);

    switch (choice) {
      case 1:
        setSamplingInterval(sampling_interval);
        break;
      case 2:
        setUltrasonicThreshold(min_threshold_ultrasonic, min_threshold_LDR);
        break;
      case 3:
        setLDRThreshold();
        break;
      case 4:
        // Exit submenu
        print_main_menu();
        break;
      default:
        Serial.println("Invalid choice. Please enter a number between 1 and 4.");
    }
  } while (choice != 4);
}

void submenuResetLoggerData() {
  printSubMenuResetLoggerData();
  int choice = readNumber(eeprom_index);

  switch (choice) {
    case 1:
      resetLoggerData(sampling_interval, min_threshold_ultrasonic, min_threshold_LDR, ultrasonic_readings, LDR_readings);
      break;
    case 2:
      // Return to the main menu
      print_main_menu();
      break;
    default:
      Serial.println("Invalid choice. Please enter 1 or 2.");
  }
}

void submenuSystemStatus() {
  int choice;
  do {
    printSubMenuSystemStatus();
    choice = readNumber(eeprom_index);

    switch (choice) {
      case 1:
        {
          printing = 1;
          displayCurrentSensorReadings(printing);
        }
        break;
      case 2:
        displayCurrentSensorSettings();
        break;
      case 3:
        displayLoggedData();
        break;
      case 4:
        // Exit submenu
        print_main_menu();
        break;
      default:
        Serial.println("Invalid choice. Please enter a number between 1 and 4.");
    }
  } while (choice != 4);
}

void submenuRGBLEDControl() {
  int choice;
  do {
    printSubMenuRGBLEDControl();
    choice = readNumber(eeprom_index);

    switch (choice) {
      case 1:
        manualColorControl();
        break;
      case 2:
        toggleAutomaticMode(automatic_mode);
        break;
      case 3:
        // Exit submenu
        print_main_menu();
        break;
      default:
        Serial.println("Invalid choice. Please enter a number between 1 and 3.");
    }
  } while (choice != 3);
}


void print_main_menu() {
  Serial.println("Hi there! Welcome to the main menu. Choose an option from below:");
  Serial.println("1. Sensor settings");
  Serial.println("2. Reset logger data");
  Serial.println("3. System status");
  Serial.println("4. RGB LED Control");
}

void printSubMenuSensorSettings() {
  Serial.println("1.1. Sensors sampling interval");
  Serial.println("1.2. Ultrasonic alert threshold");
  Serial.println("1.3. LDR alert threshold");
  Serial.println("1.4. Back");
}

void printSubMenuResetLoggerData() {
  Serial.println("Do you want to reset logger data?");
  Serial.println("2.1. YES");
  Serial.println("2.2. NO");
}

void printSubMenuSystemStatus() {
  Serial.println("3.1. Current sensors readings");
  Serial.println("3.2. Current sensors settings");
  Serial.println("3.3. Display logged data");
  Serial.println("3.4. Back");
}

void printSubMenuRGBLEDControl() {
  Serial.println("4.1. Manual color control");
  Serial.println("4.2. Toggle atutomatic mode ON/OFF");
  Serial.println("4.3. Back");
}

int readNumber(int &eeprom_index) {
  while (!Serial.available())
    ;

  int value = Serial.parseInt();

  // Read the stored value from EEPROM
  int storedValue;
  EEPROM.get(eeprom_index, storedValue);

  // Update EEPROM with the new value
  EEPROM.update(eeprom_index, value);

  eeprom_index++;

  return value;
}

void setSamplingInterval(int &sampling_interval) {
  Serial.println("Enter the sampling interval. This will be used for both sensors.");
  sampling_interval = readNumber(eeprom_index);

  Serial.print("Sampling interval is ");
  Serial.print(sampling_interval);
}

void setUltrasonicThreshold(int &min_threshhold_ultrasonic, int &min_threshold_LDR) {
  Serial.println("Enter the minimum alert threshold for the ultrasonic sensor.");
  min_threshold_ultrasonic = readNumber(eeprom_index);
  // You can use the min_threshold_ultrasonic variable as needed
}

void setLDRThreshold() {
  Serial.println("Enter the minimum alert threshold for the LDR sensor.");
  min_threshold_LDR = readNumber(eeprom_index);
  // You can handle the LDR logic here
}

void resetLoggerData(int &sampling_interval, int &min_threshold_ultrasonic, int &min_threshold_LDR, int ultrasonic_readings[], int LDR_readings[]) {
  sampling_interval = 10;
  min_threshold_ultrasonic = 2;
  min_threshold_LDR = 200;

  // Reset the ultrasonic_readings array
  for (int i = 0; i < max_readings; ++i) {
    ultrasonic_readings[i] = 0;
  }

  // Reset the LDR_readings array
  for (int i = 0; i < max_readings; ++i) {
    LDR_readings[i] = 0;
  }

  Serial.println("Data successfully reset.");
}



void displayCurrentSensorReadings(int &printing) {
  while (printing == 1) {

    sensors(photocellValue, distance, sampling_interval, ultrasonic_readings, LDR_readings, automatic_mode, index_ultrasonic, index_LDR);

    Serial.print("Analog reading = ");
    Serial.println(photocellValue);

    Serial.print("Distance: ");
    Serial.println(distance);

    delay(500);  // delay in order to display the sensor readeings in a readable way

    // Check if serial data is available (key pressed)
    if (Serial.available()) {
      readNumber(eeprom_index);  // Clear the input buffer
      break;                     // Exit the loop

      printing = 0;
    }
  }
}


void displayCurrentSensorSettings() {

  Serial.println("Here are the current sensor settings:");
  Serial.print("Sampling interval for both sensors: ");
  Serial.println(sampling_interval);
  Serial.print("Min threshold for the ultrasonic sensor: ");
  Serial.println(min_threshold_ultrasonic);
  Serial.print("Min threshold for the LDR sensor: ");
  Serial.println(min_threshold_LDR);
}

void displayLoggedData() {
  sensors(photocellValue, distance, sampling_interval, ultrasonic_readings, LDR_readings, automatic_mode, index_ultrasonic, index_LDR);
  Serial.println("Last 10 Ultrasonic Sensor Readings:");
  for (int i = 0; i < max_readings; ++i) {
    Serial.print("Reading ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(ultrasonic_readings[i]);
  }

  Serial.println("Last 10 LDR Sensor Readings:");
  for (int i = 0; i < max_readings; ++i) {
    Serial.print("Reading ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(LDR_readings[i]);
  }
}

void manualColorControl() {
  Serial.println("Insert the number of the desired color:");
  Serial.println("1. Red");
  Serial.println("2. Green");
  Serial.println("3. Blue");
  Serial.println("4. Turn the LED off");
  //EEPROM.update(9, Serial.parseInt());
  int color_choice = readNumber(eeprom_index);

  switch (color_choice) {
    case 1:
      digitalWrite(RGB_red, HIGH);
      break;
    case 2:
      digitalWrite(RGB_green, HIGH);
      break;
    case 3:
      digitalWrite(RGB_blue, HIGH);
      break;
    case 4:
      digitalWrite(RGB_red, LOW);
      digitalWrite(RGB_green, LOW);
      digitalWrite(RGB_blue, LOW);
      break;
    default:
      Serial.println("You have to enter a valid number");
  }
}

void toggleAutomaticMode(int &automatic_mode) {
  automatic_mode = 0;
  Serial.println("1. Automatic mode ON");
  Serial.println("2. Automatic mode OFF");

  int on_off_choice = readNumber(eeprom_index);

  switch (on_off_choice) {
    case 1:
      automatic_mode = 1;
      break;
    case 2:
      automatic_mode = 0;
      break;
    default:
      Serial.println("You have to enter a valid number!");
  }
}

void sensors(int &photocellValue, int &distance, int &sampling_interval, int ultrasonic_readings[], int LDR_readings[], int &automatic_mode, int &index_ultrasonic, int &index_LDR) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);

  photocellValue = analogRead(photocellPin);
  // Serial.print("Analog reading = ");
  // Serial.println(photocellValue); // the raw analog reading
  delayMicroseconds(sampling_interval);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Sound wave reflects from the obstacle, so to calculate the distance we
  // consider half of the distance traveled.
  distance = duration * 0.034 / 2;

  // Store readings first, then update indices
  ultrasonic_readings[index_ultrasonic] = distance;
  LDR_readings[index_LDR] = photocellValue;

  // Update indices
  index_ultrasonic = (index_ultrasonic + 1) % max_readings;
  index_LDR = (index_LDR + 1) % max_readings;

  if (automatic_mode == 1) {
    if (distance < min_threshold_ultrasonic || photocellValue < min_threshold_LDR) {
      digitalWrite(RGB_green, LOW);
      digitalWrite(RGB_red, HIGH);
      tone(buzzer, 100);
    } else {
      digitalWrite(RGB_red, LOW);
      digitalWrite(RGB_green, HIGH);
      noTone(buzzer);
    }
  }
}




void ultrasonic_sensor(int sampling_interval) {
  //Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(sampling_interval);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Sound wave reflects from the obstacle, so to calculate the distance we
  // consider half of the distance traveled.
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}

void LDR_sensor(int sampling_interval) {

  photocellValue = analogRead(photocellPin);
  Serial.print("Analog reading = ");
  Serial.println(photocellValue);  // the raw analog reading
  delay(sampling_interval);
}
