// Copyright (c) 2022 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#ifndef BUTTON_H_
#define BUTTON_H_

#include <M5Unified.h>

class Button : public m5::Button_Class {
  const uint8_t pin_;
 public:
  explicit Button(uint8_t pin) : pin_(pin) {}
  void begin() {
    ::pinMode(pin_, INPUT_PULLUP);
    setHoldThresh(2000);
    update();
  }
  void update() {
    setRawState(m5gfx::millis(), (::digitalRead(pin_) == HIGH));
  }
};

#endif  // BUTTON_H_
