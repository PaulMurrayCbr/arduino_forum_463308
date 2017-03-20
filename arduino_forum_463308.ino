
// Sketch for tagamo at the arduino forums, http://forum.arduino.cc/index.php?topic=463308

struct Cycler {
  const int N;
  const byte *pin;
  const float MIN_BRIGHTNESS;
  const float MAX_BRIGHTNESS;
  const float SPEED_Hz;
  const float phaseOffset;
  const float millis2radians;

  Cycler(
    int N,
    const byte pin[],
    const float MIN_BRIGHTNESS,
    const float MAX_BRIGHTNESS,
    const float SPEED_Hz) :
    N(N),
    pin(pin),
    MIN_BRIGHTNESS(MIN_BRIGHTNESS),
    MAX_BRIGHTNESS(MAX_BRIGHTNESS),
    SPEED_Hz(SPEED_Hz),
    phaseOffset(2 * PI / N),
    millis2radians(2 * PI / 1000.0 * SPEED_Hz)
  {
  }

  void setup() {
    for (int i = 0; i < N; i++) {
      pinMode(pin[i], OUTPUT);
    }
  }


  void loop() {
    float rad = millis() * millis2radians;
    for (int i = 0; i < N; i++) {
      float s = sin(rad + i * phaseOffset);
      s = (s + 1) / 2;
      s = s * (MAX_BRIGHTNESS - MIN_BRIGHTNESS) + MIN_BRIGHTNESS;
      analogWrite(pin[i], s);
    }
  }
};

struct Flasher {

  const byte pin;
  const uint32_t ON_TIME_MS;
  const uint32_t OFF_TIME_MS;
  uint32_t startMS;
  boolean on;

  Flasher(const byte pin,
          const uint32_t ON_TIME_MS,
          const uint32_t OFF_TIME_MS):
    pin(pin),
    ON_TIME_MS(ON_TIME_MS),
    OFF_TIME_MS(OFF_TIME_MS)
  {
  }

  void setup() {
    pinMode(pin, OUTPUT);

    startMS = millis();
    digitalWrite(pin, HIGH);
    on = true;
  }

  void loop() {
    uint32_t tt = millis() - startMS;
    if (on && tt > ON_TIME_MS) {
      startMS = millis();
      on = false;
      digitalWrite(pin, LOW);
    }
    else if (!on && tt > OFF_TIME_MS) {
      startMS = millis();
      on = true;
      digitalWrite(pin, HIGH);
    }
  }
};


//////////// PINOUT ////////////

const byte bussardPins[] = {3, 5, 6, 9, 10};
Cycler bussard(
  sizeof(bussardPins) / sizeof(*bussardPins),
  bussardPins,
  64, 196, // brightness range
  2.0 // speed of 2Hz.
);


Flasher flasher[] = {
  Flasher(7, 200, 800), // pin 7, on for 200, off for 800
  Flasher(8, 1000, 1000) // pin 8, 2 seconds 50/50 cycle
};

const int FLASHERS = sizeof(flasher) / sizeof(*flasher);


void setup() {
  bussard.setup();
  for (int i = 0; i < FLASHERS; i++) {
    flasher[i].setup();
  }
}


void loop() {
  bussard.loop();
  for (int i = 0; i < FLASHERS; i++) {
    flasher[i].loop();
  }
}


