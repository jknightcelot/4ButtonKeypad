#include <Arduino.h>
#include <AceButton.h>
#include "Fsm.h"

using namespace ace_button;

// function prototypes
void handleKeypadEvent(AceButton* button, uint8_t eventType, uint8_t buttonState);
void setupKeypad();
void setupLED();
void setupFSM();
void state_0_enter();
void state_0_exit();
void state_1_enter();
void state_1_exit();
void on_trans_state_0_state_1();
void on_trans_state_0_state_0();
void on_trans_state_1_state_0();
void on_trans_state_1_state_1();

#define ENABLE_SERIAL 1

const int KEYPAD_1_PIN = 2;     // KeyPad button mapped to pin
const int KEYPAD_2_PIN = 3;     // KeyPad button mapped to pin
const int KEYPAD_3_PIN = 4;     // KeyPad button mapped to pin
const int KEYPAD_4_PIN = 5;     // KeyPad button mapped to pin

// Keypad Buttons
ButtonConfig keypadConfig;
AceButton keypadButton1;
AceButton keypadButton2;
AceButton keypadButton3;
AceButton keypadButton4;

int buttonPressCount = 0;

// setup states
/// setup event ids
#define SUCCESS 0
#define FAILURE 1

/// holder of the current state
int current_state;

/// define states
State state_0(state_0_enter, NULL, &state_0_exit);
State state_1(state_1_enter, NULL, &state_1_exit);

/// define Fsm
Fsm fsm_button(&state_0);

void setup() {
  #if ENABLE_SERIAL == 1
    Serial.begin(9600);
  #endif

  setupKeypad();
  setupLED();
  setupFSM();

  #if ENABLE_SERIAL == 1
    while (! Serial); // Wait until Serial is ready - Leonardo
    Serial.println(F("Machine Online"));
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
    buttonPressCount++;
    //TODO: this needs to move to a case statement rather than if.
    if (button == &keypadButton1 && current_state == 0){
      fsm_button.trigger(SUCCESS);
      return;
    }else{
      fsm_button.trigger(FAILURE);
    }

    if (button == &keypadButton2 && current_state == 1){
      fsm_button.trigger(SUCCESS);
      return;
    }else{
      fsm_button.trigger(FAILURE);
    }
  }
}

void setupFSM(){
  // add fsm transitions
  fsm_button.add_transition(&state_0, &state_1,SUCCESS,&on_trans_state_0_state_1);
  fsm_button.add_transition(&state_0, &state_0,FAILURE,&on_trans_state_0_state_0);
  fsm_button.add_transition(&state_1, &state_0,SUCCESS,&on_trans_state_1_state_0);
  fsm_button.add_transition(&state_1, &state_1,FAILURE,&on_trans_state_1_state_1);
  // start the fsm
  fsm_button.run_machine();
}

void setupLED(){
  // initialize the LED pin as an output:
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize the pushbutton pin as an input:
  digitalWrite(LED_BUILTIN, LOW);
}

void setupKeypad(){
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
}

/// Transition callback functions
void state_0_enter()
{
  Serial.println("Entering STATE_0");
  current_state = 0;
}

void state_0_exit()
{
  Serial.println("Exiting STATE_0");
}

void state_1_enter(){
  Serial.println("Entering STATE_1");
  current_state = 1;
}

void state_1_exit(){
  Serial.println("Exiting STATE_1");
}

void on_trans_state_0_state_1()
{
  Serial.println("Transitioning from STATE_0 to STATE_1");
}

void on_trans_state_1_state_0()
{
  Serial.println("Transitioning from STATE_1 to STATE_0");
}

void on_trans_state_0_state_0(){
  Serial.println("Transitioning from STATE_0 to STATE_0");
}

void on_trans_state_1_state_1(){
  Serial.println("Transitioning from STATE_1 to STATE_1");
}
