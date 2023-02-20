
#ifndef KLOP_H
#define KLOP_H

#include "../lib/Adafruit_TCS34725_SoftI2C-master/Adafruit_TCS34725softi2c.h"
#define TCS34725_R_Coef 0.136 
#define TCS34725_G_Coef 1.000
#define TCS34725_B_Coef -0.444
#define TCS34725_GA 1.0
#define TCS34725_DF 310.0
#define TCS34725_CT_Coef 3810.0
#define TCS34725_CT_Offset 1391.0

// структура для хранения данных
struct data {
    uint16_t r, g, b, c;
    uint16_t lux;
    uint16_t temp;
};

class tcs34725 {
public:
    tcs34725() = default;
    ~tcs34725();
    boolean begin(uint8_t sdaPin = 4, uint8_t sclPin = 5);
    data* getData();

private:
  struct tcs_agc {
    tcs34725Gain_t ag{};
    tcs34725IntegrationTime_t at{};
    uint16_t mincnt{};
    uint16_t maxcnt{};
  };
    static const tcs_agc agc_lst[];
    uint16_t agc_cur = 0;

    void setGainTime();

    data* d_tcs = nullptr;
    boolean isAvailable = false, isSaturated = false;
    uint16_t againx{}, atime{}, atime_ms{};
    uint16_t saturation{}, saturation75{};
    Adafruit_TCS34725softi2c tcs{};
};

const tcs34725::tcs_agc tcs34725::agc_lst[] = {
  { TCS34725_GAIN_60X, TCS34725_INTEGRATIONTIME_700MS,     0, 20000 },
  { TCS34725_GAIN_60X, TCS34725_INTEGRATIONTIME_154MS,  4990, 63000 },
  { TCS34725_GAIN_16X, TCS34725_INTEGRATIONTIME_154MS, 16790, 63000 },
  { TCS34725_GAIN_4X,  TCS34725_INTEGRATIONTIME_154MS, 15740, 63000 },
  { TCS34725_GAIN_1X,  TCS34725_INTEGRATIONTIME_154MS, 15740, 0 }
};

// initialize the sensor
boolean tcs34725::begin(uint8_t sdaPin, uint8_t sclPin) {
  tcs = Adafruit_TCS34725softi2c(agc_lst[agc_cur].at, agc_lst[agc_cur].ag, sdaPin, sclPin);
  if ((isAvailable = tcs.begin())) 
    setGainTime();
  return(isAvailable);
}

// Set the gain and integration time
void tcs34725::setGainTime() {
  tcs.setGain(agc_lst[agc_cur].ag);
  tcs.setIntegrationTime(agc_lst[agc_cur].at);
  atime = int(agc_lst[agc_cur].at);
  atime_ms = ((256 - atime) * 2.4);  
  switch(agc_lst[agc_cur].ag) {
  case TCS34725_GAIN_1X: 
    againx = 1; 
    break;
  case TCS34725_GAIN_4X: 
    againx = 4; 
    break;
  case TCS34725_GAIN_16X: 
    againx = 16; 
    break;
  case TCS34725_GAIN_60X: 
    againx = 60; 
    break;
  }        
}

data* tcs34725::getData() {
  delete d_tcs;
  d_tcs = new data;

  tcs.getRawData(&(d_tcs->r), &(d_tcs->g), &(d_tcs->b), &(d_tcs->c));
  while(true) {
    if (agc_lst[agc_cur].maxcnt && d_tcs->c > agc_lst[agc_cur].maxcnt)
      agc_cur++;
    else if (agc_lst[agc_cur].mincnt && d_tcs->c < agc_lst[agc_cur].mincnt)
      agc_cur--;
    else break;

    setGainTime(); 
    delay((256 - atime) * 2.4 * 2); // shock absorber
    tcs.getRawData(&(d_tcs->r), &(d_tcs->g), &(d_tcs->b), &(d_tcs->c));
    break;    
  }

  // DN40 calculations
    uint16_t ir = (d_tcs->r + d_tcs->g + d_tcs->b > d_tcs->c) ? (d_tcs->r + d_tcs->g + d_tcs->b - d_tcs->c) / 2 : 0;
    uint16_t r_comp = d_tcs->r - ir;
    uint16_t g_comp = d_tcs->g - ir;
    uint16_t b_comp = d_tcs->b - ir;
//    uint16_t c_comp = d_tcs->c - ir;

  saturation = ((256 - atime) > 63) ? 65535 : 1024 * (256 - atime);
  saturation75 = (atime_ms < 150) ? (saturation - saturation / 4) : saturation;
  isSaturated = (atime_ms < 150 && d_tcs->c > saturation75);
  double cpl = (atime_ms * againx) / (TCS34725_GA * TCS34725_DF);
  double maxlux = 65535 / (cpl * 3);

    d_tcs->lux = (uint16_t)((TCS34725_R_Coef * float(r_comp) + TCS34725_G_Coef * float(g_comp) + TCS34725_B_Coef * float(b_comp)) / cpl);
    d_tcs->temp = TCS34725_CT_Coef * float(b_comp) / float(r_comp) + TCS34725_CT_Offset;
    if(d_tcs->lux > maxlux)
        d_tcs = getData();

    return d_tcs;
}

tcs34725::~tcs34725() {
    delete d_tcs;
}

#endif