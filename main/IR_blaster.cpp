#include "IR_blaster.h"

IR_blaster::IR_blaster(byte pin):ir_send(pin){
  ir_send.begin();
}

void IR_blaster::sendCode(uint32_t code) {
    uint32_t toSend;
    if(!code)
        toSend = temperature_codes[(temperature-18)];
    else
        toSend = code;

  Serial.print("SENDING CODE to LG AC: ");
  Serial.println(toSend, HEX);
  //ir_send.begin();
  ir_send.sendLG(toSend, 28);
  Serial.print("Current temperature: ");
  Serial.println(temperature);
}

void IR_blaster::register_action(JsonObject& obj){
    json = obj;
    trigger_action = true;
}

void IR_blaster::update(){

    if(!trigger_action)
        return;
    trigger_action = false;

    const char* mode = json["blast"];
    int temp = json["temp"];
    if(mode){
        if(!strcmp(mode, "up")){
            if(temperature < 30)
                temperature++;
                sendCode();
        }
        else if(!strcmp(mode, "down")){
            if(temperature > 18)
                temperature--;
                sendCode();
        }
        else if(!strcmp(mode, "on_off")){
            uint32_t toSend = (last_on) ? _SWITCH_OFF : _SWITCH_ON;
            last_on = !last_on;
            sendCode(toSend);
        }
    }
    else if(temp){
        if(temp >= 18 && temp <= 30){
            temperature = temp;
            sendCode();
        }
    }
}