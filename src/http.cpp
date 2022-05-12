// Copyright (c) 2022 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "app.h"
#include "settings.h"

constexpr static const char* NAME = "M5FS";

String templateProcessor(const String& var) {
  int index = -1;
  if (var.startsWith("A_")) index = 0;
  if (var.startsWith("B_")) index = 1;

  if (index >= 0) {
    auto code = ::strtol(var.substring(2).c_str(), nullptr, 10);
    if (code == Settings::Instance()[index]) {
      return "selected";
    }
  }
  return String();
}

class _HttpApp : public IApp {
  Button* A_ = nullptr;
  Button* B_ = nullptr;
  AsyncWebServer server_;

 public:
  _HttpApp() : server_(80) {}
  void setup(Button* a, Button* b, Adafruit_NeoPixel* led) override {
    if (a == nullptr || b == nullptr) {
      ESP.restart();
    }
    if (!start_wifi()) {
      ESP.restart();
    }

    A_ = a;
    B_ = b;
    for (auto i = 0; i < led->numPixels(); ++i) {
      led->setPixelColor(i, 0xffffff);
    }
    led->show();
  }
  void loop() override {
    if (A_->wasPressed() || B_->wasPressed()) {
      ESP.restart();
      for (;;) {}
    }
  }

 private:
  bool start_wifi() {
    if (!WiFi.mode(WIFI_AP)) {
      return false;
    }
    if (!WiFi.softAP(NAME)) {
      return false;
    }
    if (!MDNS.begin(NAME)) {
      return false;
    }
    server_.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", String(), false, templateProcessor);
    });
    server_.on("/", HTTP_POST, [](AsyncWebServerRequest *request){
      Settings::Instance().save(request);
      request->send(SPIFFS, "/index.html", String(), false, templateProcessor);
    });
    server_.begin();
    return true;
  }
};

IApp* HttpApp() {
  static _HttpApp app;
  return &app;
}
