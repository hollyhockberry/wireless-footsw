// Copyright (c) 2022 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include "settings.h"
#include <ArduinoJson.h>
#include <BleKeyboard.h>
#include <SPIFFS.h>

Settings& Settings::Instance() {
  static Settings o;
  return o;
}

void Settings::begin() {
  SPIFFS.begin(true);
  key_[0] = KEY_LEFT_ARROW;
  key_[1] = KEY_RIGHT_ARROW;
  load();
}

namespace {
const char* keys[] = { "a", "b" };

uint8_t get_argument(AsyncWebServerRequest* request, const char* name,
                     uint8_t v = 0u) {
  return request->hasArg(name)
    ? ::strtol(request->arg(name).c_str(), nullptr, 10) : v;
}
}  // namespace

bool Settings::load() {
  auto file = SPIFFS.open("/settings.json", "r");
  if (!file || file.size() == 0) {
    return false;
  }
  DynamicJsonDocument json(1300);
  auto err = ::deserializeJson(json, file);
  if (err) {
    return false;
  }
  for (auto i = 0; i < 2; ++i) {
    if (json.containsKey(keys[i])) {
      key_[i] = static_cast<uint8_t>(json[keys[i]]);
    }
  }
  return true;
}

bool Settings::save(AsyncWebServerRequest* request) {
  uint8_t n[2];
  for (auto i = 0; i < 2; ++i) {
    n[i] = get_argument(request, keys[i], key_[i]);
  }
  if ((key_[0] == n[0]) && (key_[1] == n[1])) {
    return true;
  }
  StaticJsonDocument<200> json;
  for (auto i = 0; i < 2; ++i) {
    key_[i] = n[i];
    json[keys[i]] = n[i];
  }
  auto fs = SPIFFS.open("/settings.json", "w");
  serializeJson(json, fs);
  fs.close();
  return true;
}
