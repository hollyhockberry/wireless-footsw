// Copyright (c) 2022 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "app.h"
#include "button.h"
#include "settings.h"

namespace {
Button A(PIN_A), B(PIN_B);
Adafruit_NeoPixel LED(1, PIN_LED);
IApp* App = nullptr;
}  // namespace

IApp* KeyboardApp();
IApp* HttpApp();

void setup() {
  LED.begin();
  A.begin();
  B.begin();
  Settings::Instance().begin();

  App = (A.isPressed() && B.isPressed()) ? HttpApp() : KeyboardApp();
  App->setup(&A, &B, &LED);
}

void loop() {
  A.update();
  B.update();
  App->loop();
}
