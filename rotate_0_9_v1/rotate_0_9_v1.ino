#include <Servo.h>

Servo myServo;

// --- STATE VARIABLES ---
int currentLevel = 0;   // 0-9 on display
int currentAngle = 0;   // Current position of the arm
bool movingUp = true;   // Direction of the sweep

// --- PIN DEFINITIONS ---
const int btnUp   = 2;
const int btnDown = 3;
const int btnStop = 4;
const int segPins[] = {5, 6, 7, 12, 13, A0, A1};

byte digits[10][7] = {
  {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1},
  {0,1,1,0,0,1,1}, {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0},
  {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}
};

void setup() {
  myServo.attach(9);
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnStop, INPUT_PULLUP);
  
  for (int i = 0; i < 7; i++) pinMode(segPins[i], OUTPUT);
  displayDigit(0);
}

void loop() {
  // 1. CHECK BUTTONS
  if (digitalRead(btnUp) == LOW && currentLevel < 9) {
    currentLevel++;
    displayDigit(currentLevel);
    delay(200); 
  }
  if (digitalRead(btnDown) == LOW && currentLevel > 0) {
    currentLevel--;
    displayDigit(currentLevel);
    delay(200);
  }
  if (digitalRead(btnStop) == LOW) {
    currentLevel = 0;
    displayDigit(0);
    delay(200);
  }

  // 2. AUTO-SWEEP LOGIC
  if (currentLevel > 0) {
    // Map Level 1-9 to a delay (30ms for slow, 3ms for fast)
    int stepDelay = map(currentLevel, 1, 9, 30, 3);
    
    if (movingUp) {
      currentAngle++;
      if (currentAngle >= 180) movingUp = false; // Switch direction
    } else {
      currentAngle--;
      if (currentAngle <= 0) movingUp = true;    // Switch direction
    }
    
    myServo.write(currentAngle);
    delay(stepDelay); 
  }
}

void displayDigit(int num) {
  for (int i = 0; i < 7; i++) digitalWrite(segPins[i], digits[num][i]);
}