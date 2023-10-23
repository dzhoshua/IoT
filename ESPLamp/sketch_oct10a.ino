#include "Config.h"
#include "WiFi.h"
#include "Server.h"
#include "MQTT.h"

void setup(){
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  bool is_wifi_on = init_WIFI(false);
  if(is_wifi_on){
        server_init();
        init_MQTT();
    }
    String topic = "esp8266_" + id() + "/command";
    String state_topic = "esp8266_" + id() + "/state";
    mqtt_client.subscribe(topic.c_str());
    mqtt_client.publish(state_topic.c_str(), "hello");
    Serial.println("See me at " + state_topic);

}

void loop(){
  server.handleClient();
  mqtt_client.loop();
  delay(20);
}
