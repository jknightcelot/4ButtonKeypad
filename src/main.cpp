#include <Arduino.h>
#include <AceButton.h>
#include "Fsm.h"

using namespace ace_button;

// function prototypes
void handleKeypadEvent(AceButton* button, uint8_t eventType, uint8_t buttonState);
void setupKeypad();
void setupLED();
void setupFSM();
void state_x_enter();
void state_x_exit();
void state_0_enter();
void state_0_exit();
void state_1_enter();
void state_1_exit();
void state_2_enter();
void state_2_exit();
void state_3_enter();
void state_3_exit();
void state_4_enter();
void state_4_exit();
void on_trans_state_x_state_x();
void on_trans_state_x_state_0();
void on_trans_state_0_state_1();
void on_trans_state_0_state_x();
void on_trans_state_1_state_2();
void on_trans_state_1_state_x();
void on_trans_state_2_state_3();
void on_trans_state_2_state_x();
void on_trans_state_3_state_4();
void on_trans_state_3_state_x();
void on_trans_state_4_state_0();
void on_trans_state_4_state_4();



#define ENABLE_SERIAL 0

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
#define RESET -1

/// holder of the current state
int current_state;

/// define states
State state_x(state_x_enter, NULL, &state_x_exit);
State state_0(state_0_enter, NULL, &state_0_exit);
State state_1(state_1_enter, NULL, &state_1_exit);
State state_2(state_2_enter, NULL, &state_2_exit);
State state_3(state_3_enter, NULL, &state_3_exit);
State state_4(state_4_enter, NULL, &state_4_exit);

/// define Fsm
Fsm fsm_button(&state_0);

void setup() {
  #if ENABLE_SERIAL == 0
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
    #if ENABLE_SERIAL == 1
      Serial.print("buttonPressCount = ");
      Serial.println(buttonPressCount);
    #endif
    switch(current_state) {
      case -1 : // in state x
        if (buttonPressCount == 4) {
          // display red led.
          digitalWrite(LED_BUILTIN, HIGH);
          fsm_button.trigger(RESET);
        } else {
            fsm_button.trigger(FAILURE);
        }
        break;

      case 0  : // in state 0
        if (button == &keypadButton1){
          fsm_button.trigger(SUCCESS);
        } else {
          fsm_button.trigger(FAILURE);
        }
        break;

      case 1  : // in state 1
        if (button == &keypadButton2) {
          fsm_button.trigger(SUCCESS);
        } else {
          fsm_button.trigger(FAILURE);
        }
        break;

      case 2  : // in state 2
        if (button == &keypadButton3) {
          fsm_button.trigger(SUCCESS);
        } else {
          fsm_button.trigger(FAILURE);
        }
        break;

      case 3  : // in state 3
        if (button == &keypadButton4) {
          fsm_button.trigger(SUCCESS);
        } else {
          fsm_button.trigger(FAILURE);
        }
        break;

      case 4  : // in state 3
        fsm_button.trigger(SUCCESS);
        break;

    }
  }
}

void setupFSM(){
  // add fsm transitions
  fsm_button.add_transition(&state_0, &state_1, SUCCESS, &on_trans_state_0_state_1);
  fsm_button.add_transition(&state_1, &state_2, SUCCESS, &on_trans_state_1_state_2);
  fsm_button.add_transition(&state_2, &state_3, SUCCESS, &on_trans_state_2_state_3);
  fsm_button.add_transition(&state_3, &state_4, SUCCESS, &on_trans_state_3_state_4);
  fsm_button.add_transition(&state_4, &state_4, SUCCESS, &on_trans_state_4_state_4);
  fsm_button.add_transition(&state_0, &state_x, FAILURE, &on_trans_state_0_state_x);
  fsm_button.add_transition(&state_1, &state_x, FAILURE, &on_trans_state_1_state_x);
  fsm_button.add_transition(&state_2, &state_x, FAILURE, &on_trans_state_2_state_x);
  fsm_button.add_transition(&state_3, &state_x, FAILURE, &on_trans_state_3_state_x);
  fsm_button.add_transition(&state_x, &state_x, FAILURE, &on_trans_state_x_state_x);
  fsm_button.add_transition(&state_x, &state_0, RESET, &on_trans_state_x_state_0);
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
void state_x_enter(){
  #if ENABLE_SERIAL == 1
    Serial.println("Entering STATE_x");
  #endif
  current_state = -1;
}

void state_x_exit(){
  #if ENABLE_SERIAL == 1
    Serial.println("Exiting STATE_x");
  #endif
}

void state_0_enter(){
  #if ENABLE_SERIAL == 1
    Serial.println("Entering STATE_0");
  #endif
  buttonPressCount = 0;
  current_state = 0;
}

void state_0_exit(){
  #if ENABLE_SERIAL == 1
    Serial.println("Exiting STATE_0");
  #endif
  digitalWrite(LED_BUILTIN, LOW);
}

void state_1_enter(){
  #if ENABLE_SERIAL == 1
    Serial.println("Entering STATE_1");
  #endif
  current_state = 1;
}

void state_1_exit(){
  #if ENABLE_SERIAL == 1
    Serial.println("Exiting STATE_1");
  #endif
}

void state_2_enter(){
  #if ENABLE_SERIAL == 1
    Serial.println("Entering STATE_2");
  #endif
  current_state = 2;
}

void state_2_exit(){
  #if ENABLE_SERIAL == 1
    Serial.println("Exiting STATE_2");
  #endif
}

void state_3_enter(){
  #if ENABLE_SERIAL == 1
    Serial.println("Entering STATE_3");
  #endif
  current_state = 3;
}

void state_3_exit(){
  #if ENABLE_SERIAL == 1
    Serial.println("Exiting STATE_3");
  #endif
}

void state_4_enter(){
  #if ENABLE_SERIAL == 1
    Serial.println("Entering STATE_4");
  #endif
  current_state = 4;
}

void state_4_exit(){
  #if ENABLE_SERIAL == 1
    Serial.println("Exiting STATE_4");
  #endif
}

void on_trans_state_x_state_x(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_x to STATE_x");
  #endif
}

void on_trans_state_x_state_0(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_x to STATE_0");
  #endif
}

void on_trans_state_0_state_1(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_0 to STATE_1");
  #endif
}

void on_trans_state_0_state_x(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_0 to STATE_x");
  #endif
}

void on_trans_state_1_state_2(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_1 to STATE_2");
  #endif
}

void on_trans_state_1_state_x(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_1 to STATE_x");
  #endif
}

void on_trans_state_2_state_3(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_2 to STATE_3");
  #endif
}

void on_trans_state_2_state_x(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_2 to STATE_x");
  #endif
}

void on_trans_state_3_state_4(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_3 to STATE_4");
  #endif
}

void on_trans_state_3_state_x(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_3 to STATE_x");
  #endif
}

void on_trans_state_4_state_0(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_4 to STATE_0");
  #endif
}

void on_trans_state_4_state_4(){
  #if ENABLE_SERIAL == 1
    Serial.println("Transitioning from STATE_4 to STATE_4");
  #endif
}
