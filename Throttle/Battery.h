#pragma once


class Battery {
  static const int PIN_CHG = 23;
  static const int PIN_HICHG = 22;

  SimpleKalmanFilter filter;
  public:
    Battery(): filter(1, 1, 0.1) {};

    void setup() {
      pinMode(VBAT_ENABLE, OUTPUT);
      digitalWrite(VBAT_ENABLE, LOW);
      pinMode(PIN_VBAT, INPUT);
      pinMode(PIN_CHG, INPUT);
      pinMode(PIN_HICHG, OUTPUT);
      digitalWrite(PIN_HICHG, LOW);

      analogReference(AR_DEFAULT);
      analogReadResolution(12);
    }

    float getVoltage() {
      int a = analogRead(PIN_VBAT);
      float vbatt = a * 3.6 / 4096 * 1510 / 510 * 1.04;
      // Serial.println("Battery: " + String(a));
      return filter.updateEstimate(vbatt);
    }

    int getLevel() {
      float v = getVoltage();
      if (v > 4.11)      //90%
        return 3;
      else if (v > 3.91) //65%
        return 2;
      else if (v > 3.80) //40%
        return 1;
      return 0;
    }

    int isCharging() {
      int chg = digitalRead(PIN_CHG);
      return (chg == 0);
    }
};
extern Battery battery;

