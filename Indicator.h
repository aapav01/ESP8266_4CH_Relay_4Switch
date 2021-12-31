void indicator_run();

#if !defined(BOARD_LED_BRIGHTNESS)
#define BOARD_LED_BRIGHTNESS 255
#endif

#define DIMM(x)    ((uint32_t)(x)*(BOARD_LED_BRIGHTNESS)/255)
#define RGB(r,g,b) (DIMM(r) << 16 | DIMM(g) << 8 | DIMM(b) << 0)
#define TO_PWM(x)  ((uint32_t)(x)*(BOARD_PWM_MAX)/255)

class Indicator {
public:

  enum Colors {
    COLOR_BLACK   = RGB(0x00, 0x00, 0x00),
    COLOR_WHITE   = RGB(0xFF, 0xFF, 0xE7),
    COLOR_BLUE    = RGB(0x0D, 0x36, 0xFF),
    COLOR_BLYNK   = RGB(0x2E, 0xFF, 0xB9),
    COLOR_RED     = RGB(0xFF, 0x10, 0x08),
    COLOR_MAGENTA = RGB(0xA7, 0x00, 0xFF),
  };

  Indicator() {
  }

  void init() {
    m_Counter = 0;
    initLED();
  }

  uint32_t run() {
    State currState = BlynkState::get();

    // Reset counter if indicator state changes
    if (m_PrevState != currState) {
      m_PrevState = currState;
      m_Counter = 0;
    }

    if (g_buttonPressed) {
      if (millis() - g_buttonPressTime > BUTTON_HOLD_TIME_ACTION)     { return beatLED(COLOR_WHITE,   (int[]){ 100, 100 }); }
      if (millis() - g_buttonPressTime > BUTTON_HOLD_TIME_INDICATION) { return waveLED(COLOR_WHITE,   1000); }
    }
    switch (currState) {
    case MODE_RESET_CONFIG:
    case MODE_WAIT_CONFIG:       return beatLED(COLOR_BLUE,    (int[]){ 50, 500 });
    case MODE_CONFIGURING:       return beatLED(COLOR_BLUE,    (int[]){ 200, 200 });
    case MODE_CONNECTING_NET:    return beatLED(COLOR_BLYNK,   (int[]){ 50, 500 });
    case MODE_CONNECTING_CLOUD:  return beatLED(COLOR_BLYNK,   (int[]){ 100, 100 });
    case MODE_RUNNING:           return waveLED(COLOR_BLYNK,   5000);
    case MODE_OTA_UPGRADE:       return beatLED(COLOR_MAGENTA, (int[]){ 50, 50 });
    default:                     return beatLED(COLOR_RED,     (int[]){ 80, 100, 80, 1000 } );
    }
  }

protected:

  /*
   * LED drivers
   */

void initLED() {
  pinMode(BOARD_LED_PIN, OUTPUT);
}

void setLED(uint32_t color) {
  #if BOARD_LED_INVERSE
  analogWrite(BOARD_LED_PIN, TO_PWM(255 - color));
  #else
  analogWrite(BOARD_LED_PIN, TO_PWM(color));
  #endif
}


/*
 * Animations
 */

uint32_t skipLED() {
  return 20;
}


template<typename T>
uint32_t beatLED(uint32_t, const T& beat) {
  const uint8_t cnt = sizeof(beat)/sizeof(beat[0]);
  setLED((m_Counter % 2 == 0) ? BOARD_LED_BRIGHTNESS : 0);
  uint32_t next = beat[m_Counter % cnt];
  m_Counter = (m_Counter+1) % cnt;
  return next;
}

uint32_t waveLED(uint32_t, unsigned breathePeriod) {
  uint32_t brightness = (m_Counter < 128) ? m_Counter : 255 - m_Counter;

  setLED(DIMM(brightness*2));

  // This function relies on the 8-bit, unsigned m_Counter rolling over.
  m_Counter = (m_Counter+1) % 256;
  return breathePeriod / 256;
}

private:
  uint8_t m_Counter;
  State   m_PrevState;
};

Indicator indicator;

/*
 * Animation timers
 */

#if defined(USE_TICKER)

  #include <Ticker.h>

  Ticker blinker;

  void indicator_run() {
    uint32_t returnTime = indicator.run();
    if (returnTime) {
      blinker.attach_ms(returnTime, indicator_run);
    }
  }

  void indicator_init() {
    indicator.init();
    blinker.attach_ms(100, indicator_run);
  }

#elif defined(USE_PTHREAD)

  #include <pthread.h>

  pthread_t blinker;

  void* indicator_thread(void*) {
    while (true) {
      uint32_t returnTime = indicator.run();
      returnTime = BlynkMathClamp(returnTime, 1, 10000);
      vTaskDelay(returnTime);
    }
  }

  void indicator_init() {
    indicator.init();
    pthread_create(&blinker, NULL, indicator_thread, NULL);
  }

#elif defined(USE_TIMER_ONE)

  #include <TimerOne.h>

  void indicator_run() {
    uint32_t returnTime = indicator.run();
    if (returnTime) {
      Timer1.initialize(returnTime*1000);
    }
  }

  void indicator_init() {
    indicator.init();
    Timer1.initialize(100*1000);
    Timer1.attachInterrupt(indicator_run);
  }

#elif defined(USE_TIMER_THREE)

  #include <TimerThree.h>

  void indicator_run() {
    uint32_t returnTime = indicator.run();
    if (returnTime) {
      Timer3.initialize(returnTime*1000);
    }
  }

  void indicator_init() {
    indicator.init();
    Timer3.initialize(100*1000);
    Timer3.attachInterrupt(indicator_run);
  }

#elif defined(USE_TIMER_FIVE)

  #include <Timer5.h>    // Library: https://github.com/michael71/Timer5

  int indicator_counter = -1;
  void indicator_run() {
    indicator_counter -= 10;
    if (indicator_counter < 0) {
      indicator_counter = indicator.run();
    }
  }

  void indicator_init() {
    indicator.init();
    MyTimer5.begin(1000/10);
    MyTimer5.attachInterrupt(indicator_run);
    MyTimer5.start();
  }

#else

  #warning LED indicator needs a functional timer!

  void indicator_run() {}
  void indicator_init() {}

#endif
