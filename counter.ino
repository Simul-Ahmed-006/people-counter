#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the I2C address and LCD dimensions

int ir_right_pin = A0; // in ir
int ir_left_pin = A1; // out ir

int ir_right_state = 0;
int ir_left_state = 0;

int ir_right_state_last = -1;
int ir_left_state_last = -1;

int in_counter = 0;
int out_counter = 0;

bool bWalkIn = false;
bool bWalkOut = false;
unsigned long tm;
int DELAY_TIMEOUT = 1500;

void setup() {
  Serial.begin(9600);
  pinMode(ir_right_pin, INPUT);
  pinMode(ir_left_pin, INPUT);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("In: ");
  lcd.print(in_counter);
  lcd.setCursor(0, 1);
  lcd.print("Out: ");
  lcd.print(out_counter);
}

void loop() {
  ir_right_state = digitalRead(ir_right_pin);
  ir_left_state = digitalRead(ir_left_pin);

  checkWalkIn();
  checkWalkOut();

  // Print counters to the serial monitor
  Serial.print("In: ");
  Serial.print(in_counter);
  Serial.print(" Out: ");
  Serial.println(out_counter);

  // Print counters to the LCD
  lcd.setCursor(0, 0);
  lcd.print("In: ");
  lcd.print(in_counter);
  lcd.setCursor(0, 1);
  lcd.print("Out: ");
  lcd.print(out_counter);
}

void checkWalkIn() {
  if (ir_right_state != ir_right_state_last) {
    ir_right_state_last = ir_right_state;
    if (!bWalkIn && ir_right_state == LOW) {
      bWalkIn = true;
      tm = millis();
    }
  }

  if (millis() - tm > DELAY_TIMEOUT) {
    bWalkIn = false;
  }

  if (bWalkIn && ir_left_state == LOW && ir_right_state == HIGH) {
    bWalkIn = false;
    in_counter++;
  }
}

void checkWalkOut() {
  if (ir_left_state != ir_left_state_last) {
    ir_left_state_last = ir_left_state;
    if (!bWalkOut && ir_left_state == LOW) {
      bWalkOut = true;
      tm = millis();
    }
  }

  if (millis() - tm > DELAY_TIMEOUT) {
    bWalkOut = false;
  }

  if (bWalkOut && ir_right_state == LOW && ir_left_state == HIGH) {
    bWalkOut = false;
    out_counter++;
  }
}
