#include <SimpleKalmanFilter.h>


class Rotary {
  private:
    const int VCC_PIN = 7;
    const int IN_PIN = A2;
    SimpleKalmanFilter filter;
  public:
    Rotary(): filter(1, 1, 0.1) {};
    
    void setup() {
      pinMode(IN_PIN, INPUT);
      pinMode(VCC_PIN, OUTPUT);
    }
    
    int read() {
      digitalWrite(VCC_PIN, HIGH);
      float a = analogRead(IN_PIN);
      digitalWrite(VCC_PIN, LOW);
      a = a / 930 * 100;
      return filter.updateEstimate(a);
    }
};
