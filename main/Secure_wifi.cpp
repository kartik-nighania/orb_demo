#include "Secure_wifi.h"

Secure_wifi::Secure_wifi(){
};

void Secure_wifi::init(JsonObject& json){
    ssid = json["ssid"];
    password = json["password"];
    register_action("connect");
};

void Secure_wifi::register_action(const char* type){

    if(strcmp(type, "connect") == 0){
        WiFi.begin(ssid, password);
        Serial.println("Connecting to WiFi..");
        while (WiFi.status() != WL_CONNECTED);
        Serial.println("Connected to WiFi network.");
    }
    else if(strcmp(type, "disconnect") == 0){
        WiFi.disconnect();
        Serial.println("disconnected to WiFi network.");
    }
};

boolean Secure_wifi::isConnected(){
    return WiFi.status() == WL_CONNECTED;
};