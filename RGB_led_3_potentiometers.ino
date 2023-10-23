 const int red = 3;
 const int green = 5;
 const int blue = 6;
 
void setup() {

  Serial.begin(9600);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  
}
 
void loop() {

  int intensity_red = analogRead(A0); 
  int intensity_green = analogRead(A1); 
  int intensity_blue = analogRead(A2); 

  //I used tyhe following values for mapping the voltage. The potentiometer's values range from 0 to 1024 Ohms and the led's values range from 0 to 255 Ohms.
  // Mapping means 'translating' the value read from the potentiometer to the one used by the led.
  int pot_max_value = 1024;
  int pot_min_value = 0;
  int led_max_value = 255;
  int led_min_value = 0;
 
  intensity_red = map(intensity_red, pot_min_value, pot_max_value, led_min_value, led_max_value); //mapping function
  analogWrite(red, intensity_red);
  Serial.print(intensity_red);

  intensity_green = map(intensity_green, pot_min_value, pot_max_value, led_min_value, led_max_value);
  analogWrite(green, intensity_green);
  Serial.print(intensity_green);

  intensity_blue = map(intensity_blue, pot_min_value, pot_max_value, led_min_value, led_max_value);
  analogWrite(blue, intensity_blue);
  Serial.print(intensity_blue);
}