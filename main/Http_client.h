#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <string.h>
#include <HTTPClient.h>

/* a class having both register and trigger action is not needed
  as this is intended to do an action like LED which is to get or post here
  but like a button it is not going to get triggerred as here for sure when this
  is called we need to send a set of callback of pre and post event which can be 
  taken as another thing the http has to do.. is to implement both the callback
  using its update method. This makes it compulsory to then call and implement
  callbacks at two different updates or pre and post as other peripherals will 
  then get the time to execute it. therefore register action should be given the 
  json pre and post object then to be given to call back at two different occassions
  of update.
*/
class Http_client {
  private:
    HTTPClient http;
    boolean trigger_action = false;
    void (*event_callback)(JsonObject& json);
    const byte GET = 1;
    const byte POST = 2;
    boolean pre_event_called = false;
    std::vector<byte> type_queue;
    std::vector<const char*> url_queue;
    std::vector<JsonObject> json_pre_event_queue;
    std::vector<JsonObject> json_post_event_queue;
    // Todo make callback queue later if needed and remove init
    void (*callback_func)(JsonObject& json);
    void add_to_queues(byte type, const char* url, JsonObject pre_event, JsonObject post_event);
    void remove_from_queue(unsigned long int pos=0);

  public:
    void init(void (*callback)(JsonObject& obj));
    void register_action(JsonObject& json);
    void update();
};
