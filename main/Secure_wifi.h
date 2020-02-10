#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <string.h>

class Secure_wifi {
  private:
    const char* ssid;
    const char* password;

  public:
    Secure_wifi();
    void init(JsonObject& json);
    void register_action(const char* type);
    boolean isConnected();
};