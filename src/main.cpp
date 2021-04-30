#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <NeoPixelBus.h>

const char* ssid = "CHRISTIAN_BARAJAS";
const char* password = "BotLAB2018";

const uint16_t PixelCount = 12;
const uint8_t PixelPin = 4;

#define colorSaturation 128
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
RgbColor green(0, colorSaturation, 0);
RgbColor black(0);

const int output = 2;
String sliderValue = "0";
String sliderValue2 = "0";
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

const int ledPin = 5;
String buttonValue = "0";

const char* PARAM_INPUT = "value";

AsyncWebServer server(80);

String processor(const String& var){
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}

String processorButton(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    return buttonValue;
  }
  return String();
}

void setup() {
  Serial.begin(9600);

  strip.Begin();
  strip.Show();
  strip.SetPixelColor(3, black);
  strip.Show();

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(output, ledChannel);
  ledcWrite(ledChannel, sliderValue.toInt());

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/styles.css", "text/css");
  });

  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.js");
  });

  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      ledcWrite(ledChannel, sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println("Slider1:" + inputMessage);
    request->send(200, "text/plain", "OK");
  });

  server.on("/slider2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue2 = inputMessage;
      int pixels = map(sliderValue2.toInt(), 0, 255, 0, 11);
      for (int i = 0; i <= 11; i++)
      {
        strip.SetPixelColor(i, black);
      }
      strip.Show();
      for (int i = 0; i <= pixels; i++)
      {
        strip.SetPixelColor(i, green);
      }
      strip.Show();
      if (pixels == 0)
      {
        strip.SetPixelColor(0, black);
        strip.Show();
      }
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println("Slider2: " + inputMessage);
    request->send(200, "text/plain", "OK");
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      buttonValue = inputMessage;
      digitalWrite(ledPin, buttonValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println("Button1:" + inputMessage);
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}