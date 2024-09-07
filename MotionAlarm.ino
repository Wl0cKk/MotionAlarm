#include <RCSwitch.h>

#define A 16736113
#define B 14527912
#define C 16736114
#define D 16736120

#define TRIG_PIN 3
#define ECHO_PIN 4
#define SPEAKER_PIN 5

#define TRIGGER_DISTANCE 80 // in cm

RCSwitch mySwitch = RCSwitch();

bool alarmActive = false;
bool sirenActive = false;

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0); // D2 433Mhz RX

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
}

void playOkSound() {
  tone(SPEAKER_PIN, 1000, 100);
  delay(100);
  noTone(SPEAKER_PIN);
}

void playResetSound() {
  tone(SPEAKER_PIN, 500, 100);
  delay(100);
  noTone(SPEAKER_PIN);
}

void loop() {
    if (mySwitch.available()) {
        unsigned long value = mySwitch.getReceivedValue();
        switch (value) {
            case A:
                alarmActive = true;
                Serial.println("Alarm activated");
                playOkSound();
                break;
            case B:
                alarmActive = false;
                sirenActive = false;
                noTone(SPEAKER_PIN);
                Serial.println("Alarm deactivated");
                playResetSound();
                break;
            case C:
                sirenActive = true;
                Serial.println("Siren activated");
                break;
            case D:
                sirenActive = false;
                noTone(SPEAKER_PIN);
                Serial.println("Siren deactivated");
                playResetSound();
                break;
        }
        mySwitch.resetAvailable();
    }
  
    long duration, distance;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (duration * 0.034) / 2; // distance in cm
    
    if (alarmActive && distance < TRIGGER_DISTANCE) { 
        sirenActive = true;
    }
  
    if (!sirenActive) {
        noTone(SPEAKER_PIN);
    } else {
        tone(SPEAKER_PIN, 4000);
        delay(100);
        noTone(SPEAKER_PIN);
        delay(100);
        tone(SPEAKER_PIN, 4500);
        delay(100);
        noTone(SPEAKER_PIN);
        delay(100);
    }
    delay(50);
}
