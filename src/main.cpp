/**
 * ================================================================================
 *          PROJECT: MINI C4 BOMB (GAME PROP / TIMER STYLE INSPIRED BY CS:GO)
 * ================================================================================
 * Hardware used:
 *  - Arduino Uno
 *  - 4-digit 7-segment display (3641AS - Common Cathode)
 *  - 4x4 matrix keypad (configured as 3x3 using pins A0 to A5)
 *  - Passive buzzer (connected to pin 13)
 *
 * Controls:
 *  - Key '1': Arms the bomb and starts the 30-second countdown.
 *  - Keys '7', '5', '3': Secret code sequence to DISARM the bomb.
 *  - Key '9': Resets the game after an explosion or successful disarm.
 * 
 * Vibecoded code made by Claude. We decided to share it with the community for educational purposes. AI assist us understanding how to build this code and it implemented while we didn't have enough time at school, lol
 * We made it at school and it was a fun project to learn about electronics and programming.
 * ================================================================================
 */

#include <Arduino.h>
#include <Keypad.h>

// ---------------------------------------------------------
// 1. HARDWARE PIN MAPPING
// ---------------------------------------------------------
// Segment pins (A to G) connected to digital pins 2 to 8
int segmentPins[7] = {2, 3, 4, 5, 6, 7, 8};

// Digit pins (D1 to D4) connected to digital pins 9 to 12
int digitPins[4] = {9, 10, 11, 12};

// Buzzer connection pin
int buzzerPin = 13;

// ---------------------------------------------------------
// 2. MATRIX KEYPAD CONFIGURATION (3x3)
// ---------------------------------------------------------
const byte ROWS = 3;
const byte COLUMNS = 3;

char keys[ROWS][COLUMNS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
};

// Rows R1, R2, R3 connected to analog pins A0, A1, A2
byte rowPins[ROWS] = {A0, A1, A2};

// Columns C1, C2, C3 connected to analog pins A3, A4, A5
byte columnPins[COLUMNS] = {A3, A4, A5};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, ROWS, COLUMNS);

// ---------------------------------------------------------
// 3. NUMBER TABLE FOR COMMON CATHODE DISPLAY
// ---------------------------------------------------------
// Segment order: {A, B, C, D, E, F, G}
// In common cathode mode: HIGH = LED ON / LOW = LED OFF
const int numbers[11][7] = {
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW }, // Number 0
  {LOW,  HIGH, HIGH, LOW,  LOW,  LOW,  LOW }, // Number 1
  {HIGH, HIGH, LOW,  HIGH, HIGH, LOW,  HIGH}, // Number 2
  {HIGH, HIGH, HIGH, HIGH, LOW,  LOW,  HIGH}, // Number 3
  {LOW,  HIGH, HIGH, LOW,  LOW,  HIGH, HIGH}, // Number 4
  {HIGH, LOW,  HIGH, HIGH, LOW,  HIGH, HIGH}, // Number 5
  {HIGH, LOW,  HIGH, HIGH, HIGH, HIGH, HIGH}, // Number 6
  {HIGH, HIGH, HIGH, LOW,  LOW,  LOW,  LOW }, // Number 7
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH}, // Number 8
  {HIGH, HIGH, HIGH, HIGH, LOW,  HIGH, HIGH}, // Number 9
  {LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  HIGH}  // Index 10 -> waiting dash "-"
};

// ---------------------------------------------------------
// 4. GAME CONTROL VARIABLES
// ---------------------------------------------------------
enum GameState { WAITING, ARMED, EXPLODED, DISARMED };
GameState currentState = WAITING;

int remainingSeconds = 30;          // Total game time in seconds
unsigned long previousTimer = 0;    // Tracks the real 1-second clock
unsigned long lastBeep = 0;          // Controls the accelerated sound rhythm

// Secret code to disarm the bomb
char secretCode[3] = {'7', '5', '3'};
char currentAttempt[3] = {'_', '_', '_'};
int attemptIndex = 0;

void setup() {
  // Configure display pins as outputs
  for (int i = 0; i < 7; i++) pinMode(segmentPins[i], OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH); // Keeps digits off during startup
  }

  pinMode(buzzerPin, OUTPUT);
}

// Function responsible for fast multiplexing to light up the 4 digits
void renderCustomDisplay(int d0, int d1, int d2, int d3) {
  int values[4] = {d0, d1, d2, d3};
  for (int i = 0; i < 4; i++) {
    for (int d = 0; d < 4; d++) digitalWrite(digitPins[d], HIGH); // Turn off all digits
    for (int s = 0; s < 7; s++) digitalWrite(segmentPins[s], numbers[values[i]][s]); // Draw the number
    digitalWrite(digitPins[i], LOW); // Turn on only the current digit (common cathode)
    delay(4); // Small delay to take advantage of the persistence of vision
  }
}

void loop() {
  char key = keypad.getKey();

  // ---------------------------------------------------------
  // A. KEY READING AND INPUT HANDLING
  // ---------------------------------------------------------
  if (key) {
    tone(buzzerPin, 2000, 40); // Short click feedback for any key press

    // State 1: Machine idle, waiting to arm
    if (currentState == WAITING && key == '1') {
      currentState = ARMED;
      remainingSeconds = 30;
      previousTimer = millis();
      lastBeep = millis();
      attemptIndex = 0;
    }
    // State 2: Bomb armed, accepting the code input
    else if (currentState == ARMED) {
      currentAttempt[attemptIndex] = key;
      attemptIndex++;
      tone(buzzerPin, 3500, 50); // Beep confirming the entered digit

      // When all 3 digits of the attempt are filled
      if (attemptIndex >= 3) {
        if (currentAttempt[0] == secretCode[0] && currentAttempt[1] == secretCode[1] && currentAttempt[2] == secretCode[2]) {
          // CORRECT CODE! (BOMB DEFUSED)
          currentState = DISARMED;
          tone(buzzerPin, 4000, 150);
          delay(150);
          tone(buzzerPin, 5000, 400);
        } else {
          // WRONG CODE: penalty of 5 seconds
          remainingSeconds -= 5;
          tone(buzzerPin, 300, 400); // Low-frequency alert indicating error
        }
        attemptIndex = 0; // Clear the attempt for the next input
      }
    }
    // State 3 & 4: End of game (Exploded or Disarmed) -> Press '9' to reset
    else if (currentState == EXPLODED || currentState == DISARMED) {
      if (key == '9') {
        currentState = WAITING;
        noTone(buzzerPin);
      }
    }
  }

  // ---------------------------------------------------------
  // B. TIMER STATE MACHINE AND CS:GO STYLE SOUND EFFECTS
  // ---------------------------------------------------------
  if (currentState == ARMED) {

    // Decrease the real clock every 1000 ms (1 second)
    if (millis() - previousTimer >= 1000) {
      previousTimer = millis();
      remainingSeconds--;

      if (remainingSeconds <= 0) {
        currentState = EXPLODED; // Time is up -> BOOM!
      }
    }

    // Beep acceleration system (the lower the time, the faster the sound)
    int beepInterval = 1000; // Normal: one beep per second
    if (remainingSeconds <= 10) beepInterval = 500; // Faster below 10s
    if (remainingSeconds <= 5)  beepInterval = 250; // Much faster below 5s
    if (remainingSeconds <= 2)  beepInterval = 100; // Final desperation in the last 2 seconds

    // Trigger the high-pitched beep independently of display refresh
    if (millis() - lastBeep >= beepInterval) {
      lastBeep = millis();
      tone(buzzerPin, 3500, 50);
    }
  }

  // ---------------------------------------------------------
  // C. 7-SEGMENT DISPLAY UPDATE BY STATE
  // ---------------------------------------------------------
  if (currentState == WAITING) {
    renderCustomDisplay(10, 10, 10, 10); // Draw four dashes "----"
  }
  else if (currentState == ARMED) {
    int tens = remainingSeconds / 10;
    int units = remainingSeconds % 10;
    renderCustomDisplay(0, 0, tens, units); // Display the running timer, e.g. 0025
  }
  else if (currentState == DISARMED) {
    renderCustomDisplay(0, 0, 0, 0); // Show static "0000" (Victory)
  }
  else if (currentState == EXPLODED) {
    tone(buzzerPin, 3500); // Continuous, deafening siren

    // Visual explosion effect with flashing "9999" and dashes on the screen
    if ((millis() / 100) % 2 == 0) {
      renderCustomDisplay(9, 9, 9, 9);
    } else {
      renderCustomDisplay(10, 10, 10, 10);
    }
  }
}
