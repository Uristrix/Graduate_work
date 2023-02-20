////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Created by Tsvetkov Yuriy
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include "constants.h"
#include "klop.h"

Klop klop;

[[maybe_unused]] void setup() {
    Serial.begin(115200);
    klop.begin(SDA2, SCL2, SDA1, SCL1, buttonPin, NUMBER);
}

[[maybe_unused]] void loop() {
//    delay(500);
//    klop.getData();
//    klop.draw_data();
    if(klop.trigger()) {
        Serial.println("pic");
        klop.getData();
        klop.draw_data();
    }
}