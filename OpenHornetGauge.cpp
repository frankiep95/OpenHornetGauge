
#include <Arduino.h>

#include "OpenHornetGauge.h"

byte drivePattern[] = { 0B1010, 0B1000, 0B0001, 0B0101, 0B0100, 0B0010 };

OpenHornetGauge::OpenHornetGauge(int coil1, int coil2, int coil3, int coil4, int _zeroPin,unsigned int _offset)
{
    this-> driveStep = 0;
    this->commandComplete = true;
    this->pins[0] = coil1;
    this->pins[1] = coil2;
    this->pins[2] = coil3;
    this->pins[3] = coil4;
    this->targetPos = 0;
    this->offset = _offset;
    this->motorSpeed = 2000;
    this->zeroPin = _zeroPin;
    this->position = 0;


    pinMode(zeroPin,INPUT);
    digitalWrite(zeroPin,HIGH);

    for(unsigned int a = 0; a < pinCount; a++){
        pinMode(pins[a],OUTPUT);
    }
    // delay(20);

    // home();

}

void OpenHornetGauge::drive() {
  byte mask = drivePattern[driveStep];
  for (int i = 0; i < 4; i++) {
    digitalWrite(pins[i], mask & 0x1);
    mask >>= 1;
    delayMicroseconds(motorSpeed);
  }
}

void OpenHornetGauge::decrement() {
  driveStep = (driveStep + 1) % driveCount;
  position--;
  drive();
}

void OpenHornetGauge::increment() {
  driveStep = (driveStep + 5) % driveCount;
  position++;
  drive();
}

void OpenHornetGauge::command(unsigned int value) {
  commandComplete = false;
  targetPos = value;
  Serial.println(targetPos);
}

void OpenHornetGauge::updatePos() {

  unsigned int a = map(position, 0, STEPS, 0, 65535);


  if (!commandComplete) {

    (targetPos - a) > (a - targetPos) ? decrement() : increment();

    if (abs(a - targetPos) < 100 && !commandComplete) {
      commandComplete = true;
      Serial.print("command Complete");
    }
  }
}

void OpenHornetGauge::home() {
  Serial.println("Homing...");
  while (digitalRead(zeroPin) == HIGH) {
    decrement();
  }
  if (digitalRead(zeroPin) == LOW) {
    for (int a = 0; a < 100; a++) {
      increment();
    }
  }
  while (digitalRead(zeroPin) == HIGH) {
    decrement();
    delay(20);
  }

  command(offset);
  position = 0;
  Serial.println("Homing complete");
}