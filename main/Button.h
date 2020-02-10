#include <Arduino.h>
#include <ArduinoJson.h>

class Button {

  private:
    byte pin;
    byte lastState = LOW;
    byte nowState;
    byte pressCount = 0; // click count
    unsigned long int lastTime = 0; // last time button pressed.
    const unsigned int PRESS_INTERVAL = 350;
    const unsigned int LONG_PRESS_INTERVAL = 2500;
    boolean debounce_busy = false;
    const byte NOCLICK = 0;
    const byte CLICK = 1;
    const byte DOUBLE_CLICK = 2;
    const byte TRIPLE_CLICK = 3;
    const byte LONG_PRESS = 8;
    const byte DEBOUNCE_TIME = 200;
    void (*single_click_callback)(JsonObject& json);
    void (*double_click_callback)(JsonObject& json);
    void (*long_press_callback)(JsonObject& json);
    JsonObject JSON_SC;
    JsonObject JSON_DC;
    JsonObject JSON_LONG_PRESS;
    byte isClicked(void);
    boolean trigger_action = false;

  public:
    Button(byte pin);
    void init(JsonObject& json, void (*callback_func)(JsonObject& obj));
    void update();
    void trigger_actions();
};