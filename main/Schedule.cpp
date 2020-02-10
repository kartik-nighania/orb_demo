#include "Schedule.h"

void Schedule::init(JsonObject& obj, 
                    void (*callback)(JsonObject& obj),
                    hw_timer_t** timer,
                    void (*interrupt_func)(void)) {
/*
    64 bit timers with 16 bit prescalars and 80MHz default clock
    using 65535 (2^16 bit) value for prescalar giving 1220.7 repeatition
    for 1 sec.
*/
  json = obj["events"];
  callback_func = callback;
  unsigned long int interval = obj["interval"];
  unsigned long long int frequency = 1221 * interval;
  *timer = timerBegin(0, 65535, true);
  timerAttachInterrupt(*timer, interrupt_func, true);
  timerAlarmWrite(*timer, frequency, true);
  timerAlarmEnable(*timer);
};

void Schedule::update(){
    if(!trigger_action) return;
    callback_func(json);
    trigger_action = false;
};

void Schedule::trigger_actions(){
    trigger_action = true;
};