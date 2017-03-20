

// five LEDs for the bussard collectors
const int BUSSARD_CT = 5;
const byte bussardPin[] = {A0, A1, A2, A3, A4};

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
  void setup() {
  }


  void loop() {

  }
};


//////////// PINOUT ////////////

const byte bussardPins[] = {3, 5, 6, 9, 10};
Cycler bussard(
  sizeof(bussardPins) / sizeof(*bussardPins),
  bussardPins,
  0, 255, // brightness range
  2.0 // speed of 2Hz.
);

void setup() {
  bussard.setup();
}


void loop() {
  bussard.loop();
}


