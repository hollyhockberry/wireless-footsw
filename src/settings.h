// Copyright (c) 2022 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <cstdint>
#include <ESPAsyncWebServer.h>

class Settings {
  uint8_t key_[2] = {0};
 public:
  static Settings& Instance();
  void begin();
  bool load();
  bool save(AsyncWebServerRequest* request);
  uint8_t operator[](int n) {return key_[n];}
 private:
  Settings() {}
  Settings(const Settings&);
  void operator=(const Settings&);
};

#endif  // SETTINGS_H_
