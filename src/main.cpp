#include <Arduino.h>
#include <AceButton.h>
#include "SimpleFSM.h"

using namespace ace_button;

// function prototypes
void handleKeypadEvent(AceButton* button, uint8_t eventType, uint8_t buttonState);

#define ENABLE_SERIAL 1

const int KEYPAD_1_PIN = 2;     // KeyPad button mapped to pin
const int KEYPAD_2_PIN = 3;     // KeyPad button mapped to pin
const int KEYPAD_3_PIN = 4;     // KeyPad button mapped to pin
const int KEYPAD_4_PIN = 5;     // KeyPad button mapped to pin

// const int LED_PIN = 13; currently using the built in pin.

// Keypad Buttons
ButtonConfig keypadConfig;
AceButton keypadButton1;
AceButton keypadButton2;
AceButton keypadButton3;
AceButton keypadButton4;

// define states
const SimpleFSM state0("State 0");
const SimpleFSM state1("State 1");
const SimpleFSM state2("State 2");
const SimpleFSM &currentState = state0;

void setup() {
  #if ENABLE_SERIAL == 1
    Serial.begin(9600);
  #endif

  // Simple config for the keypad buttons.
  keypadConfig.setEventHandler(handleKeypadEvent);

  // Keypad button 1
  pinMode(KEYPAD_1_PIN, INPUT_PULLUP);
  keypadButton1.setButtonConfig(&keypadConfig);
  keypadButton1.init(KEYPAD_1_PIN, HIGH, 1 /* ID */);

  // Keypad button 2
  pinMode(KEYPAD_2_PIN, INPUT_PULLUP);
  keypadButton2.setButtonConfig(&keypadConfig);
  keypadButton2.init(KEYPAD_2_PIN, HIGH, 2 /* ID */);

  // Keypad button 3
  pinMode(KEYPAD_3_PIN, INPUT_PULLUP);
  keypadButton3.setButtonConfig(&keypadConfig);
  keypadButton3.init(KEYPAD_3_PIN, HIGH, 3 /* ID */);

  // Keypad button 4
  pinMode(KEYPAD_4_PIN, INPUT_PULLUP);
  keypadButton4.setButtonConfig(&keypadConfig);
  keypadButton4.init(KEYPAD_4_PIN, HIGH, 4 /* ID */);

  // initialize the LED pin as an output:
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize the pushbutton pin as an input:
  digitalWrite(LED_BUILTIN, LOW);

  // setup states.

  /// state0


#if ENABLE_SERIAL == 1
  while (! Serial); // Wait until Serial is ready - Leonardo
  Serial.println(F("stopwatch ready"));
#endif
}

void loop() {
  keypadButton1.check();
  keypadButton2.check();
  keypadButton3.check();
  keypadButton4.check();
}

void handleKeypadEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  if (eventType == AceButton::kEventPressed){
    Serial.println("A button was pressed.");
    if (button == &keypadButton1 && currentState.getName() == state0.getName()){
      Serial.println("It was button 1.");

      return;
    }
    if (button == &keypadButton2 && currentState.getName() == state2.getName()){
      Serial.println("It was button 2.");
      Serial.println(currentState.getName());
      return;
    }
    currentState = state0;
  }
/*
  switch (eventType) {
    case AceButton::kEventPressed:

      break;
  }
  */
}
