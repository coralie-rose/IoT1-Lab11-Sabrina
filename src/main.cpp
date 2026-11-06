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

const int NUM_SAMPLES = 10;
int samples[NUM_SAMPLES];

// ----- Helper functions ----------------------------------------------
// showLevel() – A bar graph 
// Turn on LEDs 0 to level-1, and turn the others off.
void showLevel(int level) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < level) {            // should LED i be part of the bar?
      digitalWrite(LED_PINS[i], HIGH);
    }
    else {
      digitalWrite(LED_PINS[i], LOW);
    }
  }
}

void takeSamples() {
  for (int i = 0; i < NUM_SAMPLES; i++) {
    // Store one reading in element i
    samples[i] = analogRead(POT_PIN);
    delay(50);
  }
}

void printSamples() {
  for (int i = 0; i < 10; i++) {
    Serial.print("samples[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(samples[i]);
  }
}

float getAverage() {
  long sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum = sum + samples[i];      // add element i to the total
  }
  return (float)sum / NUM_SAMPLES;  // (float) keeps the decimals
}

int getMinimum() {
  int smallest = samples[0];
  for (int i = 1; i < NUM_SAMPLES; i++) {
    if (samples[i] < smallest) {
      smallest = samples[i];
    }
  }
  return smallest;
}

int getMaximum() {
  int largest = samples[0];
  for (int i = 1; i < NUM_SAMPLES; i++) {
    if (samples[i] > largest){
      largest = samples[i];
    }
  }
  return largest;
}

// Puts all the results in ONE JSON object and sends it.
void sendReport(float average, int minimum, int maximum, int level) {
  // A JsonDocument holds JSON data in memory
  JsonDocument report;

  // Add key : value pairs (Common Commands: "Add a Key and Value").
  report["device"] = "esp32-lab11";    // text
  report["average"] = average;         // number with decimals
  report["min"] = minimum;             // whole number
  report["max"] = maximum;        // whole number
  report["level"] = level;          // the key must be called level

  // Create a JSON array called "samples" then copy every element of the array into it.
  JsonArray data = report["samples"].to<JsonArray>();
  for (int i = 0; i < NUM_SAMPLES; i++) {
    data.add(samples[i]);    // add one value at the end
  }

  // Turn the document into text and send it, one value per line
  serializeJsonPretty(report, Serial);
  Serial.println();
}



void runMeasurement() {
  takeSamples();

  float average = getAverage();
  int minimum = getMinimum();
  int maximum = getMaximum();

  // Turn the average (0 to 4095) into a level from 0 to 4 LEDs
  int level = map((int)average, 0, 4096, 0, NUM_LEDS + 1);
  showLevel(level);   // show the level on the bar graph

  /* printSamples();
  Serial.print("Average: ");   Serial.println(average);
  Serial.print("Minimum: ");   Serial.println(minimum);
  Serial.print("Maximum: ");   Serial.println(maximum);
  Serial.print("Level: ");     Serial.println(level); */
  sendReport(average, minimum, maximum, level);   // send everything as JSON
}



// Part V – handleCommand() and checkSerial()
// handleCommand() – read one JSON command and do what it asks.
void handleCommand(String text) {
  JsonDocument command;

  // Read the JSON text into the document
  DeserializationError error = deserializeJson(command, text);

  // If the text is not valid JSON, print the reason and stop here.
  if (error) {
    Serial.print("JSON error: ");
    Serial.println(error.c_str());
    return;
  }

  // {"leds":[1,0,1,0]} – is there a key "leds" whose value is an array?
  if (command["leds"].is<JsonArray>()) {
    JsonArray leds = command["leds"];
    for (int i = 0; i < NUM_LEDS; i++) {
      // Element i of the JSON array (a missing element gives 0).
      int value = leds[i];
      digitalWrite(LED_PINS[i], value);  // 1 = on, 0 = off
    }
    Serial.println("LEDs updated");
  }

  // {"level":3} – is there a key "level" whose value is a whole number?
  if (command["level"].is<int>()) {
    int level = command["level"];
    showLevel(level);
    Serial.print("Level set to ");
    Serial.println(level);
  }

  // {"cmd":"report"} – read the text of "cmd" ("" if the key is missing)
  String name = command["cmd"] | "";
  if (name == "report") {
    runMeasurement();                  // take a measurement and send the report
  }
}

// The characters received so far. It is global, so it keeps its value
// between two calls of checkSerial().
String inputLine = "";

// Collect the characters one by one. When Enter arrives, pass the whole line to handleCommand().
void checkSerial() {
  while (Serial.available() > 0) {             // repeat for every character waiting
    // Take ONE character (Common Commands: "Read a Character").
    char c = Serial.read();

    if (c == '\n' || c == '\r') {            // Enter key ('\n' or '\r')
      inputLine.trim();                       // remove spaces at both ends
      if (inputLine.length() > 0) {            // ignore an empty line
        handleCommand(inputLine);
      }
      inputLine = "";                         // start again with an empty line
    }
    else {
      // Add the character at the end (Common Commands: "Add to a String").
      inputLine += c;
    }
  }
}



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

  for (int level = 0; level <= NUM_LEDS; level++) {
    // Serial.print("Level: ");
    // Serial.println(level);
    showLevel(level);
    delay(500);
  }

  //Wait for the button, then measure
  /* Serial.println("Press the button to measure...");
  while (digitalRead(BUTTON_PIN) == HIGH) {
    delay(10);
  }
  delay(50);  // ignore switch bounce

  /* Serial.println("Press the button, or send a JSON command...");
  while (digitalRead(BUTTON_PIN) == HIGH) {
    checkSerial();      // NEW: handle JSON commands while waiting for the button
    delay(10);
  }

  runMeasurement();

  // Now wait here until the button is released, so one press = one measurement.
  while (digitalRead(BUTTON_PIN) == LOW) {
    delay(10);
  }
  delay(50); // ignore switch bounce when the button is released */

}
