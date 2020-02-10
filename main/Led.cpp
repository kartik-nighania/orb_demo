#include "Led.h"

Led::Led(byte red_pin, byte green_pin, byte blue_pin) {

    this->red_pin = red_pin;
    this->blue_pin = blue_pin;
    this->green_pin = green_pin;
    init();
    all_off();
}

void Led::init() {
    pinMode(red_pin, OUTPUT);
    pinMode(blue_pin, OUTPUT);
    pinMode(green_pin, OUTPUT);
}


void Led::all_on() {
    digitalWrite(red_pin, LOW);
    digitalWrite(blue_pin, LOW);
    digitalWrite(green_pin, LOW);
}

void Led::all_off() {
    digitalWrite(red_pin, HIGH);
    digitalWrite(blue_pin, HIGH);
    digitalWrite(green_pin, HIGH);
}

void Led::register_action(const char* type, const char* colour, unsigned long int interval) {
    
    byte _type;
    byte _colour;

   if(strcmp(colour, "green") == 0)
        _colour = GREEN;
    else if(strcmp(colour, "red") == 0)
        _colour = RED;
    else if(strcmp(colour, "blue") == 0)
        _colour = BLUE;

    if(strcmp(type, "blink") == 0)
        _type = BLINK;
    else if(strcmp(type, "on") == 0)
        _type = ON;
    else if(strcmp(type, "off") == 0)
        _type = OFF;

    add_to_queues(_type, _colour, interval);
}

void Led::update() {

    cur_time = millis();
    for(unsigned long int i; i<status_queue.size(); i++) {

        // process is complete. Remove from queue and continue.
        if(status_queue[i] == END) {
            remove_from_queues(i);
            continue;
        }

        if(type_queue[i] == BLINK)
           status_queue[i] = blink(colour_queue[i], register_time_queue[i], status_queue[i], interval_queue[i]);
        else if(type_queue[i] == ON){
           on(colour_queue[i]);
           status_queue[i] = END;
        }
        else if(type_queue[i] == OFF){
           off(colour_queue[i]);
           status_queue[i] = END;
        }
    }
}

void Led::add_to_queues(byte type, byte colour, unsigned long int interval) {
    colour_queue.push_back(colour);
    type_queue.push_back(type);
    register_time_queue.push_back(millis());
    status_queue.push_back(START);
    interval_queue.push_back(interval);
}

void Led::remove_from_queues(unsigned long int pos) {
    colour_queue.erase(this->colour_queue.begin()+pos);
    type_queue.erase(this->type_queue.begin()+pos);
    register_time_queue.erase(this->register_time_queue.begin()+pos);
    status_queue.erase(this->status_queue.begin()+pos);
    interval_queue.erase(this->interval_queue.begin()+pos);
}

void Led::printall(){
    for(unsigned long int i; i<type_queue.size(); i++){
        Serial.println("type " + type_queue[i] + '\n' + 'colour ' + colour_queue[i] + '\n'+'time ' + register_time_queue[i] + '\n' + 'status ' + status_queue[i] + '\n\n');
    }
}

byte Led::blink(byte colour, unsigned long register_time, byte status, unsigned long int interval) {
    if (status == START) {
        digitalWrite(colour_to_pin(colour), LOW);
    }
    else if(status == INPROGRESS){
        unsigned long int blink_interval = (interval == 0) ? BLINK_TIME : interval;
        if((cur_time - register_time) > blink_interval){
            digitalWrite(colour_to_pin(colour), HIGH);
            return END;
        }
        // making sure other componenets dont change its state.
        digitalWrite(colour_to_pin(colour), LOW);
    }
    return INPROGRESS;
}

void Led::on(byte colour) {
    if (colour == RED)
        digitalWrite(red_pin, HIGH);
    else if(colour == GREEN)
        digitalWrite(green_pin, HIGH);
    else if(colour == BLUE)
        digitalWrite(blue_pin, HIGH);
}

void Led::off(byte colour) {
    if (colour == RED)
        digitalWrite(red_pin, LOW);
    else if(colour == GREEN)
        digitalWrite(green_pin, LOW);
    else if(colour == BLUE)
        digitalWrite(blue_pin, LOW);
}

byte Led::colour_to_pin(byte colour) {
    if(colour == RED)
        return red_pin;
    else if(colour == GREEN)
        return green_pin;
    else
        return blue_pin;
}