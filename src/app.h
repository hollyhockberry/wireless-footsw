// Copyright (c) 2022 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#ifndef APP_H_
#define APP_H_

#include <Adafruit_NeoPixel.h>
#include "button.h"

class IApp {
 public:
  virtual void setup(Button* a, Button* b, Adafruit_NeoPixel* led) = 0;
  virtual void loop() = 0;
};

#endif  // APP_H_
