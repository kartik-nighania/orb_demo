#include "Buzzer.h"

Buzzer::Buzzer(byte pin) {
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
    digitalWrite(this->pin, LOW);
}

void Buzzer::register_action(const char* type) {
    byte _type;
    if(!strcmp(type, "single_beep"))
        _type = SINGLE_BEEP;
    else if(!strcmp(type, "double_beep"))
        _type = DOUBLE_BEEP;
    else if(!strcmp(type, "triple_beep"))
        _type = TRIPLE_BEEP;
    else if(!strcmp(type, "long_beep"))
        _type = LONG_BEEP;

    add_to_queues(_type);
}

void Buzzer::update() {
    if(status_queue.empty()) return;
    buzz();
}

void Buzzer::buzz() {

    cur_time = millis();

    if (status == START) {
        digitalWrite(pin, HIGH);
        status = INPROGRESS;
    }
    else if((status == INPROGRESS) && ((cur_time - register_time) > BUZZ_TIME)){

        // pulse high for BUZZ_TIME done. Now trigger low for BUZZ_TIME.
        if(pulse_high){
            pulse_high = false;
            // dont turn off the buzzer if long beep is present.
            if(!long_beep)
                digitalWrite(pin, LOW);
        }
        else{
            pulse_high = true;
            // one cycle completed.
            count--;
            if(count < 1)
                status = END;
            else
                digitalWrite(pin, HIGH); // start another cycle
        }
    }
    else if(status == END){
        // Beep task completed. Remove from queue and continue to next.
        get_new_task();
        digitalWrite(pin, LOW);
    }
}

void Buzzer::add_to_queues(byte type) {
    register_time_queue.push(millis());
    status_queue.push(START);
    count_queue.push(type);
}

void Buzzer::remove_from_queues() {
    if(status_queue.empty()) return;
    count_queue.pop();
    status_queue.pop();
    register_time_queue.pop();
}

void Buzzer::get_new_task(){
    if(status_queue.empty()) return;
    register_time = register_time_queue.front();
    status = status_queue.front();
    count = count_queue.front();
    if(count == LONG_BEEP)
        long_beep = true;
    else
        long_beep = false;   
    remove_from_queues();
}
