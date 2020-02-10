#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ArduinoJson.h>

/*
Output device with IR LED. To control AC.
json supports: "blast": "up", "down", "on_off" or
                "temp": 18-30 temperature value
*/
class IR_blaster {

    private:
        uint32_t temperature_codes[13]={0x8808350, //18
                                        0x8808451,
                                        0x8808552,
                                        0x8808653,
                                        0x8808754,
                                        0x8808855,
                                        0x8808956,
                                        0x8808A57,
                                        0x8808B58,
                                        0x8808C59,
                                        0x8808D5A,
                                        0x8808E5B,
                                        0x8808F5C}; //30
        IRsend ir_send;
        const uint32_t _SWITCH_ON =  0x8800F43;
        const uint32_t _SWITCH_OFF = 0x88C0051;
        JsonObject json;
        boolean last_on = true; // assuming ACs initially ON.
        byte temperature = 24;
        boolean trigger_action = false;
        void init(byte pin);
        void sendCode(uint32_t code = 0);

    public:
        static const byte SWITCH_ON = 1;
        static const byte SWITCH_OFF = 2;
        static const byte TEMP_UP = 4;
        static const byte TEMP_DOWN = 8;

        IR_blaster(byte pin);
        void update();
        void register_action(JsonObject& obj);
};