// =====================================================================
// Lab 11 – Arrays, Loops and JSON
// IoT I – Dawson College – Electrical Engineering Technology
// Sabrina Fontaine
// =====================================================================
#include <Arduino.h>
#include <ArduinoJson.h>   // added with lib_deps in platformio.ini

// ----- Pins -------------------------------------------------
#define BUTTON_PIN 32  // push button (to GND)
#define POT_PIN 34    // potentiometer wiper (analog input)

// ----- Global variables -------------------------------------------
const int LED_PINS[4] = {16, 17, 18, 19};  // LED 0 first
const int NUM_LEDS = 4;      // the number of elements in LED_PINS
const int STEP_TIME = 200;   // how long each LED stays on (ms)


// Part IV – samples array


// ----- Helper functions ----------------------------------------------
// showLevel() – A bar graph 
// Turn on LEDs 0 to level-1, and turn the others off.
void showLevel(int level) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < level - 1) {            // should LED i be part of the bar?
      digitalWrite(LED_PINS[i], HIGH);
    }
    else {
      digitalWrite(LED_PINS[i], LOW);
    }
  }
}


// Part IV – takeSamples(), printSamples(), getAverage(),
//           getMinimum(), getMaximum()


// Part V – sendReport()


// Part IV – runMeasurement()


// Part V – handleCommand() and checkSerial()


void setup() {
  Serial.begin(115200);

  // Inputs and outputs
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // ONE for loop sets all four LED pins as outputs 
  for (int i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  Serial.println("Lab 11 - Arrays, Loops and JSON");
}

void loop() {
  // Part III – LED patterns (for loops)
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(LED_PINS[i], HIGH);   // turn LED number i on
    Serial.print("LED ");
    Serial.print(i);
    Serial.print(" is on GPIO ");
    Serial.println(LED_PINS[i]);
    delay(STEP_TIME);
    digitalWrite(LED_PINS[i], LOW);    // turn it off again
  }

  for (int i = 3; i >= 0; i--) {
    digitalWrite(LED_PINS[i], HIGH);
    delay(STEP_TIME);
    digitalWrite(LED_PINS[i], LOW);
  }

  // Commented out to allow testing of just first two for loops
  /* for (int level = 0; level <= NUM_LEDS; level++) {
    showLevel(level);
    delay(500);
  } */


  // Part IV – Wait for the button, then measure (while loops)


}
