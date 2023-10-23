#include <PubSubClient.h>

PubSubClient mqtt_client(wifiClient);

void callback(char *topic, byte *payload, unsigned int length){
  Serial.print("There is message on topic ");
  Serial.println(topic);
  Serial.println("Message is: ");
  for(int i=0; i<length;i++){
    Serial.print((char) payload[i]);
  }
  int state = (int)payload[0]-(int)'0';
  Serial.println(state);
  digitalWrite(LED_BUILTIN, state);
  Serial.println("");
}

bool init_MQTT(){
  mqtt_client.setServer(mqtt_broker, mqtt_port); //broker
  mqtt_client.setCallback(callback);
  while(!mqtt_client.connected()){
    String client_id = "esp8266_" + id();
    if(mqtt_client.connect(client_id.c_str())){
      Serial.println("MQTT client connected with id " + client_id);
    }else{
      Serial.println("Failed to connect MQTT " + client_id);
      delay(500);
    }
  }
  return true;
}
