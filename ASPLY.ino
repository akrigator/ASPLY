#include <AnalogMultiButton.h>

#define BUTTONS_PIN A0
#define LED_LIGHT   5
#define LED_FAN1    6
#define LED_FAN2    7
#define LED_FAN3    8
#define CTRL_LIGHT  9
#define CTRL_FAN1   10
#define CTRL_FAN2   11
#define CTRL_FAN3   12

#define TIMER_ACTIVATION_DELAY 2500
#define TIMER_ACTIVATION_BLINK 500
#define TIMER_BLINK            1000
#define TIMER_BASE             600000

enum Channels {CHANNEL_LIGHT, CHANNEL_FAN1, CHANNEL_FAN2, CHANNEL_FAN3, CHANNELS_TOTAL};
const int BUTTONS_VALUES[CHANNELS_TOTAL] = {40, 343, 511, 613};

AnalogMultiButton buttons(BUTTONS_PIN, CHANNELS_TOTAL, BUTTONS_VALUES);

struct Output {
  const byte pin;
  unsigned long blink_timer;
  unsigned int blink_count;
  byte state;
  Output(byte pinv): pin(pinv), state(LOW), blink_timer(0), blink_count(0) {};
  void toggle(byte n_state) {state = n_state; digitalWrite(pin, state);};
  void on() {toggle(HIGH);};
  void off() {toggle(LOW); blink_count = 0;};
  void toggle() {toggle(state ? LOW : HIGH);};
  unsigned int blinking(unsigned long period) {
    unsigned long currentMillis = millis();
    if (currentMillis - blink_timer >= period) {
      blink_timer = currentMillis;
      toggle();
      blink_count++;
    }
    return blink_count/2;
  };
};

enum ChannelState {OFF, ON, TIMER_ACTIVATION, TIMER};

struct Channel {
  Output indicator;
  Output control;
  ChannelState state;
  unsigned long timer;
  unsigned int timer_multiply;
  
  Channel(byte led, byte ctrl): indicator(Output(led)), control(Output(ctrl)), state(OFF), timer(0) {
    pinMode(led, OUTPUT);
    pinMode(ctrl, OUTPUT);
  };
  void turnOn() {
    state = ON;
    indicator.on();
    control.on();
  };
  void turnOff() {
    state = OFF;
    indicator.off();
    control.off();
  };
  void timerActivation() {state = TIMER_ACTIVATION; indicator.blinking(TIMER_ACTIVATION_BLINK);};
  void startTimer() {state = TIMER; timer_multiply = indicator.blinking(TIMER_BLINK); control.on(); timer = millis();};
  void toggle() {
    if (state == OFF)
      turnOn();
    else 
      turnOff();
  };
  void tick() {
    unsigned long currentMillis = millis();
    if (timer > 0 && currentMillis - timer >= (timer_multiply * TIMER_BASE)) {
      timer = 0;
      turnOff();
    }
  };
  void update() {if (state > ON) {indicator.blinking(TIMER_BLINK); tick();}};
  
};

struct {
  Channel channels [CHANNELS_TOTAL] = {
    Channel(LED_LIGHT, CTRL_LIGHT),
    Channel(LED_FAN1, CTRL_FAN1),
    Channel(LED_FAN2, CTRL_FAN2),
    Channel(LED_FAN3, CTRL_FAN3)
  };
  void turnOffFans(byte j) {
    if (j != CHANNEL_LIGHT) {
      for (byte i = CHANNEL_FAN1; i < CHANNELS_TOTAL; i++) if (j != i) channels[i].turnOff();
    }
  };
} asply;

void setup() {}

void loop() {
  buttons.update();
  
  for (byte i = 0; i < CHANNELS_TOTAL; i++) {

    if (buttons.isPressed(i)) {
      if (buttons.isPressedAfter(i, TIMER_ACTIVATION_DELAY))
        asply.channels[i].timerActivation();
      else
        asply.channels[i].indicator.on();
    }

    if (buttons.onReleaseAfter(i, TIMER_ACTIVATION_DELAY)) {
      if (asply.channels[i].state != TIMER) {
        asply.turnOffFans(i);
        asply.channels[i].startTimer();
      }
    } else if (buttons.onRelease(i)) {
      asply.turnOffFans(i);
      asply.channels[i].toggle();
    }
    
    asply.channels[i].update();
  }

  delay(10);
}
