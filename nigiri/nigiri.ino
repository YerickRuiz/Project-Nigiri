#include <Adafruit_PWMServoDriver.h>
#include <LiquidCrystal.h>

// PCA9685 values
#define SERVOMIN 100   // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 500   // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50  // Analog servos run at ~50 Hz updates
// Servo IDs
#define SERVO_X_ID 0     // Servo Base
#define SERVO_Y_ID 1     // Servo Reach
#define SERVO_Z_ID 2     // Servo Height
#define SERVO_G_ID 3     // Servo Gripper
#define SERVO_MAX_IDS 4  // Max number of servos
// Servo Min, Max and Default Values
#define SERVO_STEP 5
#define SERVO_X_MIN 0
#define SERVO_X_DEF 90
#define SERVO_X_MAX 180

#define SERVO_Y_MIN 40
#define SERVO_Y_DEF 50
#define SERVO_Y_MAX 150

#define SERVO_Z_MIN 30
#define SERVO_Z_DEF 90
#define SERVO_Z_MAX 160

#define SERVO_G_MIN 0
#define SERVO_G_DEF 45
#define SERVO_G_MAX 100

#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

#define SEQ_DELAY 200

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

uint8_t selected_servo = SERVO_Z_ID;
uint8_t servo_value_array[SERVO_MAX_IDS] = { SERVO_X_DEF, SERVO_Y_DEF, SERVO_Z_DEF, SERVO_G_DEF };
uint8_t servo_max_array[SERVO_MAX_IDS] = { SERVO_X_MAX, SERVO_Y_MAX, SERVO_Z_MAX, SERVO_G_MAX };
uint8_t servo_min_array[SERVO_MAX_IDS] = { SERVO_X_MIN, SERVO_Y_MIN, SERVO_Z_MIN, SERVO_G_MIN };
char servo_char_array[SERVO_MAX_IDS] = { 'X', 'Y', 'Z', 'G' };

int read_LCD_buttons() {
  int adc_key_in = analogRead(0);  // read the value from the sensor
  //Serial.println(adc_key_in);
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE;  // We make this the 1st option for speed reasons since it will be the most likely result
  if (adc_key_in < 50) return btnRIGHT;
  if (adc_key_in < 195) return btnUP;
  if (adc_key_in < 380) return btnDOWN;
  if (adc_key_in < 500) return btnLEFT;
  if (adc_key_in < 750) return btnSELECT;
  return btnNONE;  // when all others fail, return this...
}

void print_serial_servo_value() {
  Serial.print(servo_char_array[selected_servo]);
  Serial.print(":");
  Serial.println(servo_value_array[selected_servo]);
}

void push_select() {

  Serial.println("you push the button select");
  uint8_t array_size = 30;
  uint8_t movements_x[array_size];
  uint8_t movements_y[array_size];
  uint8_t movements_z[array_size];
  uint8_t movements_g[array_size];

  generate_sweep_array(movements_x, array_size, SERVO_X_DEF, SERVO_X_MAX*0.8);
  execute_movements(movements_x, array_size, SERVO_X_ID);
  generate_sweep_array(movements_x, array_size, SERVO_X_MAX*0.8, SERVO_X_DEF);
  execute_movements(movements_x, array_size, SERVO_X_ID);

  generate_sweep_array(movements_y, array_size, SERVO_Y_DEF, SERVO_Y_MAX*0.8);
  execute_movements(movements_y, array_size, SERVO_Y_ID);
  generate_sweep_array(movements_y, array_size, SERVO_Y_MAX*0.8, SERVO_Y_DEF);
  execute_movements(movements_y, array_size, SERVO_Y_ID);

  generate_sweep_array(movements_z, array_size, SERVO_Z_DEF, SERVO_Z_MAX*0.8);
  execute_movements(movements_z, array_size, SERVO_Z_ID);
  generate_sweep_array(movements_z, array_size, SERVO_Z_MAX*0.8, SERVO_Z_DEF);
  execute_movements(movements_z, array_size, SERVO_Z_ID);

  generate_sweep_array(movements_g, array_size, SERVO_G_DEF, SERVO_G_MAX*0.8);
  execute_movements(movements_g, array_size, SERVO_G_ID);
  generate_sweep_array(movements_g, array_size, SERVO_X_MAX*0.8, SERVO_G_DEF);
  execute_movements(movements_g, array_size, SERVO_G_ID);
}

void generate_sweep_array(uint8_t positions[], uint8_t size, uint8_t min_pos, uint8_t max_pos) {

  int range = max_pos - min_pos;
  int step = range / size;
  int value = min_pos;

  for (int i = 0; i < size; i++){
    positions[i] = value;
    value = value + step;
  }
}

void increment_selected_servo_value() {

  if (servo_value_array[selected_servo] <= (servo_max_array[selected_servo] - SERVO_STEP)) {

    servo_value_array[selected_servo] += SERVO_STEP;
    print_serial_servo_value();
  }
}

void decrement_selected_servo_value() {

  if (servo_value_array[selected_servo] >= (servo_min_array[selected_servo] + SERVO_STEP)) {

    servo_value_array[selected_servo] -= SERVO_STEP;
    print_serial_servo_value();
  }
}

void increment_selected_servo() {

  if (selected_servo < (SERVO_MAX_IDS - 1)) {

    selected_servo += 1;
  }
}

void decrement_selected_servo() {

  if (selected_servo > SERVO_X_ID) {

    selected_servo -= 1;
  }
}

void execute_movements(uint8_t positions[], int size_array, int servo) {
  for (int i = 0; i < size_array; i++) {
    servo_value_array[servo] = positions[i];
    delay(SEQ_DELAY);
    apply_pwm_value();
  }
}

void detect_buttons() {

  int lcd_key = read_LCD_buttons();
  switch (lcd_key)  // depending on which button was pushed, we perform an action
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
        push_select();
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

void apply_pwm_value() {

  for (int i = 0; i < SERVO_MAX_IDS; i++) {

    int result_pwm = map(servo_value_array[i], 0, 180, SERVOMIN, SERVOMAX);
    pwm.setPWM(i, 0, result_pwm);
  }
}

void setup() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  lcd.begin(16, 2);            // Configure LCD of 16 cols and 2 rows
  lcd.clear();                 // LCD screen clear
  pinMode(10, OUTPUT);         // sets backlight pin-10 as PWM output
  analogWrite(10, 125);        // Set backlight to 50% brightness
  Serial.begin(9600);
  Serial.println("Project Nigiri");
}

void loop() {
  print_display_values();
  detect_buttons();
  apply_pwm_value();
  delay(100);
}
