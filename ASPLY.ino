//ATTINY PINS
#define INPUT_PIN     2
#define LIGHT_PIN     0
#define FAN_1_PIN     1
#define FAN_2_PIN     2
#define FAN_3_PIN     3
//INPUT MODE
#define LIGHT_MODE    0
#define FAN_MODE      1

#define INPUT_BUTTONS   4
#define ANALOG_SMOOTH   5
#define DEBONCE_TIMEOUT 50

struct Control {
  char mode;
  const int analogValue;
  const int chanelPin;
  int chanelState;
  int buttonState;
  int lastButtonState;
  long time;
  bool deboncing;
};

Control controls[INPUT_BUTTONS] = {
  {LIGHT_MODE, 807, LIGHT_PIN, LOW, LOW, LOW, 0, false}, // 270 Om
  {FAN_MODE,   676, FAN_1_PIN, LOW, LOW, LOW, 0, false}, // 510 Om
  {FAN_MODE,   511, FAN_2_PIN, LOW, LOW, LOW, 0, false}, // 1 kOm
  {FAN_MODE,   338, FAN_3_PIN, LOW, LOW, LOW, 0, false}, // 2 kOm
};

void setup () {
  pinMode(LIGHT_PIN,OUTPUT);
  pinMode(FAN_1_PIN,OUTPUT);
  pinMode(FAN_2_PIN,OUTPUT);
  pinMode(FAN_3_PIN,OUTPUT);
} 

void loop () {
  int reading = analogRead(INPUT_PIN);
  for (int i = 0; i < INPUT_BUTTONS; i++) {
    if (abs(controls[i].analogValue - reading) < ANALOG_SMOOTH) {
      controls[i].buttonState = HIGH;
      if (!controls[i].lastButtonState) // button is pressed
        controls[i].time = millis();    // start debounce timer
    } else
      controls[i].buttonState = LOW;
      
    if (!controls[i].buttonState 
        && controls[i].lastButtonState 
        && (millis()-controls[i].time > DEBONCE_TIMEOUT)) {// button is release && debounce is timeout
      int currentState = controls[i].chanelState;
      if (controls[i].mode == FAN_MODE)         // safe toggle of fan
        for (int j = 0; j <INPUT_BUTTONS; j++)  // turn off all fans
          if (controls[j].mode == FAN_MODE)
            digitalWrite(controls[j].chanelPin, controls[j].chanelState = 0);
      digitalWrite(controls[i].chanelPin, controls[i].chanelState = !currentState); // toogle current chanel
    }
        
    controls[i].lastButtonState = controls[i].buttonState; // update button state
  }
}
