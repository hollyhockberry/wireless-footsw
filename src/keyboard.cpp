// Copyright (c) 2022 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <BleKeyboard.h>
#include "app.h"
#include "settings.h"

class _KeyboardApp : public IApp {
  BleKeyboard Key_;
  Button* Btn_[2] = {};
  Adafruit_NeoPixel* LED_ = nullptr;
  uint32_t color_ = 0;

 public:
  _KeyboardApp() : Key_("M5FS") {}

  void setup(Button* a, Button* b, Adafruit_NeoPixel* led) override {
    Btn_[0] = a;
    Btn_[1] = b;
    LED_ = led;
    Key_.begin();
    show(0xff0000);
  }
  void loop() override {
    if ((Btn_[0]->wasHold() && Btn_[1]->isHolding()) ||
        (Btn_[1]->wasHold() && Btn_[0]->isHolding())) {
      Key_.end();
      ESP.restart();
      for (;;) {}
    }
    uint32_t color = 0;
    if (Key_.isConnected()) {
      for (auto i = 0; i < 2; ++i) {
        if (Btn_[i] == nullptr) continue;
        if (Btn_[i]->wasPressed()) {
          Key_.write(Settings::Instance()[i]);
        }
        color |= Btn_[i]->isPressed() ? 0xff << (i * 8) : 0;
      }
    } else {
      color = 0xff0000;
    }
    show(color);
  }

 private:
  void show(uint32_t color) {
    if (LED_ == nullptr || color_ == color) return;
    for (auto i = 0; i < LED_->numPixels(); ++i) {
      LED_->setPixelColor(i, color);
    }
    LED_->show();
    color_ = color;
  }
};

IApp* KeyboardApp() {
  static _KeyboardApp app;
  return &app;
}
