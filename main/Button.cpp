#include "Button.h"

Button::Button(byte pin) {
  this->pin = pin;
  pinMode(pin, INPUT_PULLDOWN);
  update();
};

void Button::init(JsonObject& json, void (*callback_func)(JsonObject& obj)) {
    for (JsonObject::iterator pair = json.begin(); pair != json.end(); ++pair) {
    
     const char* type = pair->key().c_str();

    if(strcmp(type, "click") == 0){
      single_click_callback = callback_func;
      JSON_SC = pair->value();
    }
    else if(strcmp(type, "double_click") == 0){
      double_click_callback = callback_func;
      JSON_DC = pair->value();
    }
    else if(strcmp(type, "long_press") == 0){
      long_press_callback = callback_func;
      JSON_LONG_PRESS = pair->value();
    }
  }
};

void Button::update() {

  byte status = isClicked();
  if(status == NOCLICK) return;
  Serial.print("status ");
  Serial.println(status);

  if(!trigger_action) return;
  if((status == LONG_PRESS) && long_press_callback){
    long_press_callback(JSON_LONG_PRESS);
  }
  else if((status == CLICK) && single_click_callback){
    single_click_callback(JSON_SC);
  }
  else if((status == DOUBLE_CLICK) && double_click_callback){
    double_click_callback(JSON_DC);
  }
  trigger_action = false;
};

void Button::trigger_actions(){
  trigger_action = true;
};

byte Button::isClicked(void) {
  byte nowState = digitalRead(pin);

  // during state change
  if(nowState != lastState){
    // check debouncing
    if((millis() - lastTime) < DEBOUNCE_TIME)
      return NOCLICK;

    // pressed
    if(nowState == HIGH) {
      lastTime = millis();
      pressCount++;
    }
  }

  // below happens everytime except debounce
  if(pressCount != 0){

    // check for HOLD
    // button pressed once and still in pressed state more than 'long hold' timing.
    if((pressCount == 1) && ((millis()-lastTime) > LONG_PRESS_INTERVAL) && (nowState == HIGH)){
      lastState = nowState;
      pressCount = 0;
      return LONG_PRESS;
    }

    // check for CLICKS
    // if wait time for next click is over and button is in released state.
    if(((millis()-lastTime) > PRESS_INTERVAL) && (nowState == LOW)){
      byte presses = pressCount;
      pressCount = 0;
      return presses;
    }
  }

  lastState = nowState;
      // if no clickstate has been returned.
  return NOCLICK;
};