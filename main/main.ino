#include <ArduinoJson.h>
#include <string.h>
#include "Led.h"
#include "Button.h"
#include "Schedule.h"
#include "Http_client.h"
#include "Secure_wifi.h"
#include "IR_blaster.h"

#define LED_0_R_PIN 2
#define LED_0_G_PIN 0
#define LED_0_B_PIN 4

#define LED_1_R_PIN 16
#define LED_1_G_PIN 17
#define LED_1_B_PIN 5

#define BUTTON_0_PIN 39
#define BUTTON_1_PIN 33
#define BUTTON_2_PIN 14

#define IR_PIN 19

Led leds[2] = { Led(LED_0_R_PIN, LED_0_G_PIN, LED_0_B_PIN), 
                Led(LED_1_R_PIN, LED_1_G_PIN, LED_1_B_PIN)
              };

Button buttons[3] = { Button(BUTTON_0_PIN),
                      Button(BUTTON_1_PIN),
                      Button(BUTTON_2_PIN) 
                    };

Secure_wifi secure_wifi;
IR_blaster ir_blaster(IR_PIN);
Schedule schedule;
Http_client http_client;
hw_timer_t *timer = NULL;

StaticJsonDocument<2048> config;
const char* json_text = "{\"wifi_config\":{\"ssid\":\"venky3t-wifi\",\"password\":\"\"},\"schedule0\":{\"interval\":5,\"events\":{\"http\":{\"type\":\"get\",\"url\":\"http://remote-builder:115c084e4dec6e254eab37fa5cff5e21c0@192.168.43.235:8080/job/wireless_notification_device/lastBuild/api/json\",\"post_event\":{\"100\":{\"led\":[0,\"blue\",\"blink\"]},\"201\":{\"led\":[0,\"green\",\"blink\"]},\"400\":{\"led\":[0,\"red\",\"blink\"]}}}}},\"button0\":{\"click\":{\"ir\":{\"blast\":\"up\"},\"led\":[1,\"red\",\"blink\"]},\"double_click\":{\"led\":[1,\"blue\",\"blink\"]}},\"button2\":{\"click\":{\"ir\":{\"blast\":\"down\"},\"led\":[1,\"blue\",\"blink\"]},\"long_press\":{\"http\":{\"type\":\"post\",\"url\":\"http://remote-builder:115c084e4dec6e254eab37fa5cff5e21c0@192.168.43.235:8080/job/wireless_notification_device/build\",\"pre_event\":{\"led\":[0,\"blue\",\"blink\"]},\"post_event\":{\"success\":{\"led\":[0,\"green\",\"blink\"]},\"fail\":{\"led\":[0,\"red\",\"blink\"]}}}}},\"button1\":{\"long_press\":{\"ir\":{\"blast\":\"on_off\"},\"led\":[1,\"green\",\"blink\"]}}}";
DeserializationError json_error = deserializeJson(config, json_text);
JsonObject json = config.as<JsonObject>();


void IRAM_ATTR button0() {
  buttons[0].trigger_actions();
};

void IRAM_ATTR button1() {
  buttons[1].trigger_actions();
};

void IRAM_ATTR button2() {
  buttons[2].trigger_actions();
};

void IRAM_ATTR schedule_interrupt() {
  schedule.trigger_actions();
};

void setup() {

  Serial.begin(250000);
  while (!Serial) continue;
  Serial.println("serial monitor established.");
  
  if(json_error){
    Serial.println("json deserialization failed with error: ");
    Serial.println(json_error.c_str());
  }       

  init_and_interrupts();

  Serial.println("interrupts created.");

  if(json.containsKey("wifi_config")) {
    JsonObject wifi_config = json["wifi_config"];
    secure_wifi.init(wifi_config);
    http_client.init(callback_func);
  }
};

void loop() {
  leds[0].update();
  leds[1].update();
  buttons[0].update();
  buttons[1].update();
  buttons[2].update();
  schedule.update();
  http_client.update();
  ir_blaster.update();
};

void init_and_interrupts() {
  if (json.containsKey("button0")) {
    pinMode(BUTTON_0_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BUTTON_0_PIN), button0, RISING); // TODO changed this
    JsonObject obj = json["button0"];
    buttons[0].init(obj, callback_func);
    Serial.println("button0 init and interrupt done");
  }
  if (json.containsKey("button1")) {
    pinMode(BUTTON_1_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BUTTON_1_PIN), button1, RISING);
    JsonObject obj = json["button1"];
    buttons[1].init(obj, callback_func);
    Serial.println("button1 init and interrupt done");
  }
  if (json.containsKey("button2")) {
    pinMode(BUTTON_2_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BUTTON_2_PIN), button2, RISING);
    JsonObject obj = json["button2"];
    buttons[2].init(obj, callback_func);
    Serial.println("button2 init and interrupt done");
  }
  if (json.containsKey("schedule0")) {
    JsonObject obj = json["schedule0"];
    schedule.init(obj, callback_func, &timer, schedule_interrupt);
    Serial.println("scheduler init and interrupt done");
  }
};

int count = 0;
void callback_func(JsonObject& json){

  for (JsonObject::iterator pair = json.begin(); pair != json.end(); ++pair) {
      const char* key = pair->key().c_str();

      // TODO: yet to add events.
      if(strcmp(key, "led") == 0){
        JsonArray arr = pair->value();
        int led_number = arr[0];
        const char* colour = arr[1];
        const char* type = arr[2];

        if(arr.size() == 4) {
          int interval = arr[3];
          leds[led_number].register_action(type, colour, interval);
        }
        else {
          leds[led_number].register_action(type, colour);
        }
      }
      else if(strcmp(key, "ir") == 0){
        Serial.println("ir called and registered.");
        JsonObject obj = pair->value();
        ir_blaster.register_action(obj);
      }
      else if(strcmp(key, "buzzer") == 0){
        Serial.println("buzzer called and registered.");
      }
      else if(strcmp(key, "http") == 0){
        JsonObject obj = pair->value();
        http_client.register_action(obj);
      }
      // TODO: add comment of connect and disconnect here
      // else if(strcmp(key, "wifi") == 0){
      //   const char* type = pair->value();
      //   secure_wifi.register_action(type);
      // }
  }
  
};
