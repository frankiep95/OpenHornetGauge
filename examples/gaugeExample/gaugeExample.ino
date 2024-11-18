#include "OpenHornetGauge.h"

// Create an instance of the gauge give it a name i.e. "test"
//  arguments are coil 1 pin,coil 2 pin, coil 3 pin, coil 4 pin, zero pin, zero offset(unsigned int)
OpenHornetGauge test(21, 20, 2, 19, 3, 1200);

void setup()
{
  // starts the serial port. In this test mode you can use it to send a new position to the gauge 0-65535
  Serial.begin(115200);
  while (!Serial){}
  // Home the gauge. If the gauge is already homed it will move off home and  slowly move back
  test.home();
}

void loop()
{
  

  // update is what makes the motor move. This function is non blocking.
  //  It will increment the motor once per loop cycle.
  test.updatePos();

  static int nextPos = 0;
  if (Serial.available())  { //this function just checks if there is Serial available
  //  and converts serial.read() to an int and sends it to .command()

    char c = Serial.read();
    if (c == 10 || c == 13)
    {
      //  the command function is how you pass the new position to the gauge
      // this takes any where from 0 -65535 as DCS Bios will output that range of values
      // the library will choose the shortest path from current position to new position
      test.command(nextPos);
      nextPos = 0;
    }
    else if (c >= '0' && c <= '9')
    {
      nextPos = 10 * nextPos + (c - '0');
    }
  }
}
