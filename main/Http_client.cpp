#include "Http_client.h"

void Http_client::init(void (*callback)(JsonObject& obj)){
    callback_func = callback;
};

void Http_client::register_action(JsonObject& json){

    byte type;
    const char* request_type = json["type"];
    if(strcmp(request_type, "get")==0){
        type = GET;
    }
    else if(strcmp(request_type, "post")==0){
        type = POST;
    }

    add_to_queues(type, json["url"], json["pre_event"], json["post_event"]);
}

void Http_client::update(){
    if (type_queue.size() < 1) 
        return;

    if(!pre_event_called){
        callback_func(json_pre_event_queue[0]);
        pre_event_called = true;
    }
    else {
        // TODO: POST EVENT. post Led mixes up with pre. Had to introduce delay in led
        if((type_queue[0] == POST) && (WiFi.status() == WL_CONNECTED)){
            http.begin(url_queue[0]);
            int httpCode = http.POST("");
            String response = http.getString();

            // TODO: place to check respose value and send callback
            JsonObject obj;
            if(httpCode == 201){
                obj = json_post_event_queue[0]["success"];
                Serial.println("POST: success");
            }
            else {
                obj = json_post_event_queue[0]["fail"];
                Serial.println("POST: fail");
            }
            callback_func(obj);
            http.end();
        }
        else if((type_queue[0] == GET) && (WiFi.status() == WL_CONNECTED)){

            http.begin(url_queue[0]);
            int httpCode = http.GET();

            //Check the status of the GET request
            if (httpCode > 0) {
                
                String response = http.getString();

                // TODO: check and send callback using status 100, 201, 400
                // here using the json parsed from api
                // Variable to store response data
                // callback_func(json_post_event_queue[0]);
                StaticJsonDocument<2048> config;
                DeserializationError json_error = deserializeJson(config, response);
                JsonObject json = config.as<JsonObject>();

                if(json_error){
                    Serial.println("GET deserialization of failed with error: ");
                    Serial.println(json_error.c_str());
                }  
                else{
                    Serial.print("GET successful with httpcode: ");
                    Serial.println(httpCode);

                    const char* value = json["result"];
                    JsonObject obj;
                    if(value == NULL){
                        Serial.println("INPROGRESS");
                        obj = json_post_event_queue[0]["100"];
                        callback_func(obj);
                    }
                    else if(!strcmp(value, "SUCCESS")){
                        Serial.println(value);
                        obj = json_post_event_queue[0]["201"];
                        callback_func(obj);
                    }
                    else if(!strcmp(value, "FAILURE")){
                        Serial.println(value);
                        obj = json_post_event_queue[0]["400"];
                        callback_func(obj);
                    }
                }
            }
            // else{
            //     //Serial.println("GET response Failed");
            // }
            http.end();
        }

        // clean and reset
        remove_from_queue();
        pre_event_called = false;
    }
};

void Http_client::add_to_queues(byte type, const char* url, JsonObject pre_event, JsonObject post_event){
    type_queue.push_back(type);
    url_queue.push_back(url);
    json_pre_event_queue.push_back(pre_event);
    json_post_event_queue.push_back(post_event);
}

void Http_client::remove_from_queue(unsigned long int pos){
    type_queue.erase(this->type_queue.begin()+pos);
    url_queue.erase(this->url_queue.begin()+pos);
    json_pre_event_queue.erase(this->json_pre_event_queue.begin()+pos);
    json_post_event_queue.erase(this->json_post_event_queue.begin()+pos);
};