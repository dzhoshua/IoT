#include <ESP8266WebServer.h>

ESP8266WebServer server(WEB_SERVER_PORT);
void handle_root(){
    String page_code = "<form action=\"/CONNECT\" method=\"POST\">";
    page_code += "<input type=\"text\" name=\"ssid\" placeholder=\"ssid\">";
    page_code += "<input type=\"passw\" name=\"passw\" placeholder=\"password\">";
    page_code += "<input type=\"submit\"></form>";
    server.send(200, "text/html", page_code);
}

void handle_not_found(){
    server.send(404, "text/html", "404: check URL");
}

void handle_connect() {
    if (server.hasArg("ssid") && server.hasArg("passw")) {
        String ssid = server.arg("ssid");
        String passw = server.arg("passw");
        Serial.println("Connecting to: " + ssid);
        server.send(200, "text/html", "Connected");  

        bool res = init_WIFI(false, ssid.c_str(), passw.c_str());
        if (res == true){
            Serial.println("connect MQTT");
            init_MQTT();
            String topic = "esp8266/zlataveronika";
            mqtt_client.subscribe(topic.c_str());
    }
        delay(1000);
    }   else {
        server.send(200, "text/html", "Login or password error.");
    }
}

void server_init(){
    server.on("/", HTTP_GET, handle_root);
    server.on("/CONNECT", HTTP_POST, handle_connect);
    server.onNotFound(handle_not_found);
    server.begin();
    Serial.print("Server started on port ");
    Serial.println(WEB_SERVER_PORT);
}
