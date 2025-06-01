#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include "EPD.h"

#define IMAGE_WIDTH 296
#define IMAGE_HEIGHT 128
#define IMAGE_SIZE (IMAGE_WIDTH * IMAGE_HEIGHT / 8)

WebServer server(80);
const char* AP_SSID = "ESP32_Config";

uint8_t imageBuffer[IMAGE_SIZE];
File fsUploadFile;

void handleRoot() {
  server.send(200, "text/html", "<form method='POST' action='/upload' enctype='multipart/form-data'>"
                                   "<input type='file' name='file'>"
                                   "<input type='submit' value='Upload'></form>");
}

void handleUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    fsUploadFile = SPIFFS.open("/txt.bin", FILE_WRITE);
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
      server.send(200, "text/plain", "Upload OK");
      ESP.restart();
    } else {
      server.send(500, "text/plain", "File write failed");
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH); // Power on e-paper

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed");
    return;
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/upload", HTTP_POST, [](){}, handleUpload);
  server.begin();

  File file = SPIFFS.open("/txt.bin", FILE_READ);
  if (!file || file.size() != IMAGE_SIZE) {
    Serial.println("No valid /txt.bin found");
    return;
  }
  file.read(imageBuffer, IMAGE_SIZE);
  file.close();

  EPD_Init();
  EPD_HW_RESET();
  EPD_ShowPicture(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT, imageBuffer, BLACK);
  EPD_DisplayImage(imageBuffer);
  EPD_FastUpdate();
  EPD_Sleep();
}

void loop() {
  server.handleClient();
}
