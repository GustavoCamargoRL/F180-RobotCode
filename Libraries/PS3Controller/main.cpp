#include <QCoreApplication>
#include "joystick.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Joystick joystick("/dev/input/js0");
    //Joystick joystick;
      // Ensure that it was found and that we can use it
      if (!joystick.isFound())
      {
        printf("open failed.\n");
        exit(1);
      }

      while (true)
      {
        // Restrict rate
        usleep(1000);

        // Attempt to sample an event from the joystick
        JoystickEvent event;
        if (joystick.sample(&event))
        {
          if (event.isButton())
          {
            printf("Button %u is %s\n",
              event.number,
              event.value == 0 ? "up" : "down");
          }
          else if (event.isAxis())
          {
            printf("Axis %u is at position %d\n", event.number, event.value);
          }
        }
      }
    return a.exec();


}
