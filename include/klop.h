//
// Created by Yuriy Tsvetkov on 10.01.2023.
//

#ifndef KLOP_KLOP_H
#define KLOP_KLOP_H


#include <cstdint>

#include "../lib/GyverOLED-main/src/GyverOLED.h"
#include "tcs34725autorange_lib.h"
class Klop {
public:
    // структуры данных для 2 датчиков
    data *d_front = nullptr;
    data *d_back = nullptr;

    // объекты для работы дисплея и датчиков
    GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
    tcs34725 tcs_front;
    tcs34725 tcs_back;
    // переменные для работы кнопки и пин
    bool flag = false;
    uint32_t timer = 0;
    uint8_t button{};

    //Серийный номер прибора
    String number;

public:
    // конструктор
    Klop() noexcept = default;
    ~Klop();

    // инициализация класса
    void begin(uint8_t sda_front,
               uint8_t scl_front,
               uint8_t sda_back,
               uint8_t scl_back,
               uint8_t button_pin,
               String serial = "01"
                       );

    // метод для триггера кнопки
    bool trigger();

    // методы для отрисовки
    void draw_data();
    void draw_init();
    void draw_markup();
    void erase_data();

    // метод получение данных с датчиков
    void getData();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Klop::begin(uint8_t sda_front,
                 uint8_t scl_front,
                 uint8_t sda_back,
                 uint8_t scl_back,
                 uint8_t button_pin,
                 String serial
                 ) {
    button = button_pin;
    number = std::move(serial);

    pinMode(buttonPin, INPUT_PULLUP);

    tcs_front = tcs34725();
    tcs_back  = tcs34725();

    tcs_front.begin(sda_front, scl_front);
    tcs_back .begin(sda_back, scl_back);

    oled.init();
    draw_init();
    delay(5000);
    oled.clear();
    draw_markup();
}

void Klop::draw_data() {
    erase_data();
    if(d_front != nullptr) {
        oled.setCursor(26, 3);
        oled.print(d_front->lux);
        oled.setCursor(26, 4);
        oled.print(d_front->temp);
        oled.setCursor(26, 5);
        oled.print(d_front->r);
        oled.setCursor(26, 6);
        oled.print(d_front->g);
        oled.setCursor(26, 7);
        oled.print(d_front->b);
    }
    if(d_back != nullptr) {
        oled.setCursor(92, 3);
        oled.print(d_back->lux);
        oled.setCursor(92, 4);
        oled.print(d_back->temp);
        oled.setCursor(92, 5);
        oled.print(d_back->r);
        oled.setCursor(92, 6);
        oled.print(d_back->g);
        oled.setCursor(92, 7);
        oled.print(d_back->b);
    }
}

void Klop::draw_init() {
    oled.clear();
    oled.setScale(2);
    oled.setCursor(5, 0);
    oled.print("КЛОП-Л(" + number + ")");
    oled.setScale(1);
    oled.setCursor(35, 3);
    oled.print("Loading...");

    oled.setCursor(0, 5);
    oled.print("Created by Tsvetkov Y.");

    oled.setCursor(0, 7);
    uint8_t git[8]{0x3c, 0x5e, 0xa3, 0x03, 0x03, 0xe3, 0x7e, 0x3c};
    for(unsigned char & i : git) oled.drawByte(pgm_read_byte(&i));
    oled.setCursor(12, 7);
    oled.print(": UriStrix");

}

void Klop::draw_markup() {
    uint8_t c[8]{0x78, 0x48, 0x48, 0x0, 0x0, 0x0, 0x0, 0x0};

    oled.line(0, 1, 128, 1);
    oled.line(0, 20, 128, 20);
    oled.line(64, 1, 64, 64);

    oled.setScale(1);
    oled.setCursor(15, 1);
    oled.print("front");
    oled.setCursor(85, 1);
    oled.print("back");

    oled.setCursor(1, 3);
    oled.print("Lux: ");
    oled.setCursor(1, 4);
    oled.print("T");
    oled.setCursor(7, 4);
    for(unsigned char & i : c) oled.drawByte(pgm_read_byte(&i));
    oled.setCursor(13, 4);
    oled.print(":");
    oled.setCursor(1, 5);
    oled.print("R: ");
    oled.setCursor(1, 6);
    oled.print("G: ");
    oled.setCursor(1, 7);
    oled.print("B: ");

    oled.setCursor(67, 3);
    oled.print("Lux: ");
    oled.setCursor(67, 4);
    oled.print("T");
    oled.setCursor(73, 4);
    for(unsigned char & i : c) oled.drawByte(pgm_read_byte(&i));
    oled.setCursor(79, 4);
    oled.print(":");
    oled.setCursor(67, 5);
    oled.print("R: ");
    oled.setCursor(67, 6);
    oled.print("G: ");
    oled.setCursor(67, 7);
    oled.print("B: ");
}

void Klop::erase_data() {
    oled.clear(26, 24, 60, 60);
    oled.clear(92, 24, 128, 60);
}

//button trigger
bool Klop::trigger() {
    bool btState = !digitalRead(button);

    if(btState && !flag && millis() - timer > 100) {
        flag = true;
        timer = millis();
    }

    if(!btState && flag && millis() - timer > 100) {
        flag = false;
        timer = millis();
        return true;
    }
    return false;
}

void Klop::getData() {
    d_front = tcs_front.getData();
    d_back = tcs_back.getData();
}

Klop::~Klop() {
    delete d_back;
    delete d_front;
}

#endif //KLOP_KLOP_H
