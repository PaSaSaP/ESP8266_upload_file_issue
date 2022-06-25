#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// server config
const char* ssid = "<YOUR SSID>";
const char* password = "<YOUR PASSWORD>";

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "It works!");  
}

void handleUploadFinish() {
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", "DONE");
}

void handleUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("upload start, filename: %s\r\n", upload.filename.c_str());
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    //
  } else if (upload.status == UPLOAD_FILE_END) {
    Serial.println("upload end");
  }
}

void connectWiFi() {
  WiFi.disconnect();
  delay(1200);
  WiFi.mode(WIFI_STA);

  IPAddress staticIp(192, 168, 55, 180);
  IPAddress gatewayIp(192, 168, 55, 1);
 if (!WiFi.config(staticIp, gatewayIp, IPAddress(255, 255, 255, 0))) {
   Serial.println("Cannot set config");
 } 

  Serial.println(F("connectWiFi"));
  WiFi.begin(ssid, password);

  // wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  
  Serial.println(F(""));
  Serial.print(F("Connected to "));
  Serial.println(ssid);
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void setup(void) {
  Serial.begin(115200);
  Serial.println(F("Starting..."));
  connectWiFi();

  server.on("/", handleRoot);
  server.on("/upload", HTTP_POST, handleUploadFinish, handleUpload);

  server.begin();
  Serial.println(F("HTTP server started"));
}

void loop(void) {
  server.handleClient();
}
