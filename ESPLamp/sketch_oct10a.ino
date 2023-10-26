#include "Config.h"
#include "WiFi.h"
#include "Server.h"
#include "MQTT.h"

void setup(){
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  bool is_ap_on = start_AP_mode();
  //bool is_wifi_on = init_WIFI(false);
  if(is_ap_on){
        server_init();
        //init_MQTT();
    }
}

void loop(){
  server.handleClient();
  if (mqtt_client.connected()){
    mqtt_client.loop();
  }
  delay(20);
}
