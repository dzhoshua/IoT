#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;
WiFiClient wifiClient;

String ip = "IP not set";

String id(){
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String mac_id = String(mac[WL_MAC_ADDR_LENGTH-2], HEX) + String(mac[WL_MAC_ADDR_LENGTH-1], HEX);

  return mac_id;
}

bool start_AP_mode(){
  String ssid_id = AP_NAME + "_" + id();
  IPAddress ap_IP(192, 168, 4, 1);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ap_IP, ap_IP, IPAddress(255, 255, 255, 0));
  WiFi.softAP((ssid_id).c_str(), AP_PASSWORD.c_str());
  Serial.println("WiFi started in AP mode " + ssid_id);
  return true;
}

bool start_client_mode(){
  wifiMulti.addAP(CLI_SSID, CLI_PASS);
  while(wifiMulti.run() != WL_CONNECTED){
    delay(10);
  }
  return true;
}

bool init_WIFI(bool AP_mode){
  if(AP_mode){
    start_AP_mode();
    ip = WiFi.softAPIP().toString();
  }else{
    start_client_mode();
    ip = WiFi.localIP().toString();
  }
  Serial.println("IP address: " + ip);
  return true;
}


