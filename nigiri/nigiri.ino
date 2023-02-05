#include <LiquidCrystal.h>
// Servo IDs
#define SERVO_X_ID 0     // Servo Base
#define SERVO_Y_ID 1     // Servo Reach
#define SERVO_Z_ID 2     // Servo Height
#define SERVO_G_ID 3     // Servo Gripper
#define SERVO_MAX_IDS 4  // Max number of servos
// Servo Min, Max and Default Values
#define SERVO_X_MIN 10
#define SERVO_X_DEF 60
#define SERVO_X_MAX 170

#define SERVO_Y_MIN 10
#define SERVO_Y_DEF 60
#define SERVO_Y_MAX 170

#define SERVO_Z_MIN 10
#define SERVO_Z_DEF 60
#define SERVO_Z_MAX 170

#define SERVO_G_MIN 30
#define SERVO_G_DEF 60
#define SERVO_G_MAX 150

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
uint8_t selected_servo = SERVO_Z_ID;
uint8_t servo_value_array[SERVO_MAX_IDS] = { SERVO_X_DEF, SERVO_Y_DEF, SERVO_Z_DEF, SERVO_G_DEF };
uint8_t servo_max_array[SERVO_MAX_IDS] = { SERVO_X_MAX, SERVO_Y_MAX, SERVO_Z_MAX, SERVO_G_MAX};
uint8_t servo_min_array[SERVO_MAX_IDS] = { SERVO_X_MIN, SERVO_Y_MIN, SERVO_Z_MIN, SERVO_G_MIN};
char servo_char_array[SERVO_MAX_IDS] = { 'X', 'Y', 'Z', 'G' };

int read_LCD_buttons()
{
 int adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1500) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 500)  return btnLEFT; 
 if (adc_key_in < 700)  return btnSELECT;   
 return btnNONE;  // when all others fail, return this...
}

void increment_selected_servo_value()
{

  if(servo_value_array[selected_servo] < servo_max_array[selected_servo]){

    servo_value_array[selected_servo] +=1;
  
  }

}

void decrement_selected_servo_value(){

  if(servo_value_array[selected_servo] > servo_min_array[selected_servo]){

    servo_value_array[selected_servo] -=1;

  }

}

void increment_selected_servo(){

  if(selected_servo < (SERVO_MAX_IDS -1)){

    selected_servo +=1;

  }

}

void decrement_selected_servo(){

  if(selected_servo > SERVO_X_ID){

    selected_servo -=1;

  }

}

void detect_buttons(){

  int lcd_key = read_LCD_buttons();
  switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     increment_selected_servo();
     break;
     }
   case btnLEFT:
     {
     decrement_selected_servo();
     break;
     }
   case btnUP:
     {
     increment_selected_servo_value();
     break;
     }
   case btnDOWN:
     {
     decrement_selected_servo_value();
     break;
     }
   case btnSELECT:
     {
     
     break;
     }
     case btnNONE:
     {
     break;
     }
 }
}

void print_display_values() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.setCursor(2, 0);
  lcd.print(servo_char_array[selected_servo]);

  // Show the values for X, Y, Z and G in the LCD Screen

  lcd.setCursor(4, 0);
  lcd.print("X:");
  lcd.setCursor(6, 0);
  lcd.print(servo_value_array[SERVO_X_ID]);

  lcd.setCursor(10, 0);
  lcd.print("Y:");
  lcd.setCursor(12, 0);
  lcd.print(servo_value_array[SERVO_Y_ID]);

  lcd.setCursor(4, 1);
  lcd.print("Z:");
  lcd.setCursor(6, 1);
  lcd.print(servo_value_array[SERVO_Z_ID]);

  lcd.setCursor(10, 1);
  lcd.print("G:");
  lcd.setCursor(12, 1);
  lcd.print(servo_value_array[SERVO_G_ID]);
}


void setup() {
  lcd.begin(16, 2);      // Configure LCD of 16 cols and 2 rows
  lcd.clear();           // LCD screen clear
  pinMode(10, OUTPUT);   // sets backlight pin-10 as PWM output
  analogWrite(10, 125);  // Set backlight to 50% brightness
}


void loop() {
  print_display_values();
  detect_buttons();
  delay(100);
}