
#define DCSBIOS_DEFAULT_SERIAL

#include <DcsBios.h>
#include "Arduino.h"
#include <OpenHornetGauge.h>

// Create an instance of the gauge give it a name i.e. "test"
//  arguments are coil 1 pin,coil 2 pin, coil 3 pin, coil 4 pin, zero pin, zero offset(unsigned int)
int coil1 = 21;
int coil2 = 2;
int coil3 = 20;
int coil4 = 19;
int zero = 3;
// this function takes into account the incorrectly labeled coils on the openhornet analog gauge pcb
OpenHornetGauge test(coil1, coil2, coil3, coil4, zero);

void onPressureAltChange(unsigned int newValue) {
  test.command(newValue);
}
DcsBios::IntegerBuffer pressureAltBuffer(FA_18C_hornet_PRESSURE_ALT, onPressureAltChange);

void setup() {


  test.setRotationDegrees(315);  // the total swing of the needle. make 360 if continuous revolution
  test.setMotorSpeed(10000);      // motor speed setting the lower the number the faster the motor. usually below 800 is too fast for the motor to respond.
  test.setOffset(800);          // this is the offset to move the needle to the correct 0 position after homing

  test.home();                   // Home the gauge. If the gauge is already homed it will move off home and  slowly move back.this needs to be called after all the other parameters above

  DcsBios::setup();
}

void loop() {

  // update is what makes the motor move. This function is non blocking.
  //  It will increment the motor once per loop cycle.
  test.updatePos();
  DcsBios::loop();
}
