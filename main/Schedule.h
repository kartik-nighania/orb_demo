#include <Arduino.h>
#include <ArduinoJson.h>

class Schedule {
  private:
    void (*callback_func)(JsonObject& json);
    JsonObject json;
    boolean trigger_action = false;
  public:
    void init(JsonObject& obj, 
              void (*callback)(JsonObject& obj),
              hw_timer_t** timer,
              void (*interrupt_func)(void));
    void update();
    void trigger_actions();
};