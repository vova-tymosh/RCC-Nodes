#include <SimpleKalmanFilter.h>


class Rotary {
  private:
    const int VCC_PIN = 7;
    const int IN_PIN = A1;
    SimpleKalmanFilter filter;
  public:
    Rotary(): filter(1, 1, 0.1) {};
    
    void setup() {
      pinMode(IN_PIN, INPUT);
      pinMode(VCC_PIN, OUTPUT);
      digitalWrite(VCC_PIN, HIGH);
    }
    
    int read() {
      float a = analogRead(IN_PIN);
      a = a / 930 * 100;
      int b = a / 2;
      b *= 2;
      if (b > 100)
        b = 100;
      return b;
    }
};
