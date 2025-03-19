/*
 * RCC Example Locomotive
 *
 *
 */
#define RCC_NO_STATION
#include "Peripheral.h"
#include "Motherboard.h"
#include "RCCLoco.h"
#include "Storage.h"
#include "Settings.h"
#include "Timer.h"
#include "Audio.h"
#include "audio_data2.h"


Storage storage;
PinExt yellow(2);
Pin blue(D0);
PowerMeter powerMeter;
Motor motor(PIN_MOTOR_BCK, PIN_MOTOR_FWD);
Timer timer;
Timer blinker(1000);

Audio audio;


const int PAGE_SIZE = 256;
uint8_t page[PAGE_SIZE];
char soundFile[] = "sound";

void writeAllAudio(const uint8_t *data, const size_t size) {
    uint32_t offset = 0;
    storage.allocate(soundFile, size);

    while (offset < size) {
        uint32_t s = size - offset;
        if (s > sizeof(page))
            s = sizeof(page);
        memcpy(page, data + offset, s);
        int r = storage.write(soundFile, page, s, offset);
        offset += r;
    }
}

class TestLoco : public RCCLoco
{
public:
    using RCCLoco::RCCLoco;

    void onFunction(char code, bool value)
    {
        // Serial.println("onFunction: " + String((int)code) + "/" + String(value));
        if (code == 0)
            yellow.apply(value);
        if (code == 1)
            blue.apply(value);
    }

    void onThrottle(uint8_t direction, uint8_t throttle)
    {
        motor.apply(direction, throttle);
    }

    void onCommand(uint8_t code, float value)
    {
        switch (code) {
        case 'V':
            Serial.println(powerMeter.readVoltage());
            break;
        case 'C':
            Serial.println(powerMeter.readCurrent());
            break;
        case 'B':
            Serial.println(motor.readBemf());
            break;
        case 'P':
            audio.play(audio_data, sizeof(audio_data), 2);
            break;
        case 'Q':
            audio.play("sound", 2);
            break;
        case 'W':
            writeAllAudio(audio_data, sizeof(audio_data));
            break;
        }
    }
};
TestLoco loco;



void setup()
{
    Serial.begin(115200);
    delay(250);

    storage.begin();
    settings.begin();

    motor.setup();
    yellow.begin();
    blue.begin();
    powerMeter.setup();
    timer.start(100);
    blinker.restart();
    
    audio.begin();
    loco.setup();
}

void loop()
{
    loco.loop();
    audio.loop();

    if (blinker.hasFired()) {
        static bool flip = false;
        flip = !flip;
        if (flip) {
            // digitalWrite(LED_BUILTIN, HIGH);
            // Serial.println("Loco:");
        } else {
            // digitalWrite(LED_BUILTIN, LOW);
        }
    }
}

