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

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
uint8_t selected_servo = SERVO_Z_ID;
uint8_t servo_value_array[SERVO_MAX_IDS] = { SERVO_X_DEF, SERVO_Y_DEF, SERVO_Z_DEF, SERVO_G_DEF };
char servo_char_array[SERVO_MAX_IDS] = { 'X', 'Y', 'Z', 'G' };

void print_display_values() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.setCursor(2, 0);
  lcd.print(servo_char_array[selected_servo]);

  // Show the values for X, Y, Z and G in the LCD Screen

  lcd.setCursor(6, 0);
  lcd.print("X:");
  lcd.setCursor(8, 0);
  lcd.print(servo_value_array[SERVO_X_ID]);

  lcd.setCursor(12, 0);
  lcd.print("Y:");
  lcd.setCursor(14, 0);
  lcd.print(servo_value_array[SERVO_Y_ID]);

  lcd.setCursor(6, 1);
  lcd.print("Z:");
  lcd.setCursor(8, 1);
  lcd.print(servo_value_array[SERVO_Z_ID]);

  lcd.setCursor(12, 1);
  lcd.print("G:");
  lcd.setCursor(14, 1);
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
  delay(100);
}