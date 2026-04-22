#include <Servo.h>

Servo myServo;

// --- STATE VARIABLES ---
int currentLevel = 0;   
int currentAngle = 0;   
bool movingUp = true;   
unsigned long lastMoveTime = 0; // For non-blocking timing

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
  myServo.write(currentAngle); // Start at 0
}

void loop() {
  // 1. CHECK BUTTONS (Instant Response)
  if (digitalRead(btnUp) == LOW && currentLevel < 9) {
    currentLevel++;
    displayDigit(currentLevel);
    delay(200); // Simple debounce
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

  // 2. NON-BLOCKING SWEEP LOGIC
  if (currentLevel > 0) {
    int stepDelay = map(currentLevel, 1, 9, 30, 5); // 5ms is safer for MG90S
    
    // Only move if enough time has passed
    if (millis() - lastMoveTime >= stepDelay) {
      lastMoveTime = millis(); // Reset the timer
      
      if (movingUp) {
        currentAngle++;
        if (currentAngle >= 180) movingUp = false;
      } else {
        currentAngle--;
        if (currentAngle <= 0) movingUp = true;
      }
      
      myServo.write(currentAngle);
    }
  }
}

void displayDigit(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], digits[num][i]);
  }
}