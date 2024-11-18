
#include <Arduino.h>

#include "OpenHornetGauge.h"

byte drivePattern[] = { 0B1010, 0B1000, 0B0001, 0B0101, 0B0100, 0B0010 };

OpenHornetGauge::OpenHornetGauge(int coil1, int coil2, int coil3, int coil4, int _zeroPin)
{
    this-> driveStep = 0;
    this->commandComplete = true;
    this->pins[1] = coil1;
    this->pins[3] = coil2;
    this->pins[0] = coil3;
    this->pins[2] = coil4;
    this->targetPos = 0;
    this->offset = 0;
    this->motorSpeed = 2000;
    this->zeroPin = _zeroPin;
    this->position = 0;
    this->rotationDegrees = 360;
    this->STEPS = rotationDegrees * 3;
    this->lastUpdate = 0;
    this->ready = false;
    
    pinMode(zeroPin,INPUT);
    digitalWrite(zeroPin,HIGH);

    for(unsigned int a = 0; a < pinCount; a++){
        pinMode(pins[a],OUTPUT);
    }

}

void OpenHornetGauge::drive() {
  byte mask = drivePattern[driveStep];
  for (int i = 0; i < 4; i++) {
    digitalWrite(pins[i], mask & 0x1);
    mask >>= 1;
    // delayMicroseconds(motorSpeed);
    lastUpdate = micros();
    ready = false;
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
  if(value != position){
  commandComplete = false;
  targetPos = value;
  Serial.println(targetPos);
  }
}

void OpenHornetGauge::updatePos() {

  unsigned int a = map(position, 0, STEPS, 0, 65535);
  if(micros() - lastUpdate > motorSpeed){ready = true;}

  if (!commandComplete && ready) {

    (targetPos - a) > (a - targetPos) ? decrement() : increment();

    if (abs(a - targetPos) < 100 && !commandComplete) {
      commandComplete = true;
      Serial.println("command Complete");
    }
  }
}

void OpenHornetGauge::home() {
  Serial.println("Homing...");
  while (digitalRead(zeroPin) == HIGH) {
    decrement();
    delay(10);
  }
  if (digitalRead(zeroPin) == LOW) {
    for (int a = 0; a < 100; a++) {
      increment();
      delay(10);
    }
  }
  while (digitalRead(zeroPin) == HIGH) {
    decrement();
    delay(20);
  }

  this->position = 0;
  unsigned int temp = motorSpeed;
  motorSpeed = 30000;
  command(offset);
  while(!commandComplete){updatePos();}

  this->position = 0;
  Serial.println("Homing complete");
  motorSpeed = temp;
}

void OpenHornetGauge::setMotorSpeed(unsigned int value){
  this->motorSpeed = value;
}

void OpenHornetGauge::setRotationDegrees(unsigned int value){
  this->rotationDegrees = value;
  this->STEPS = rotationDegrees * 3;
}

void OpenHornetGauge::setOffset(unsigned int value){
  this->offset = value;
}
