#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include "string.h"

#define GPIO_PATH "/sys/class/gpio/"

class GPIO
{
  private:
    std::string directionPath;
    std::string valuePath;
    std::string edgesPath;

    int pin;
    int ledFD;
    int ledValue;

    void setDirectionPath()
    {
      char buff[100];
      snprintf(buff, sizeof(buff), "%sgpio%d/direction", GPIO_PATH, pin);
      this->directionPath = buff;
    }

    void setValuePath()
    {
      char buff[100];
      snprintf(buff, sizeof(buff), "%sgpio%d/value", GPIO_PATH, pin);
      this->valuePath = buff;
    }

    void setEdgePath()
    {
      char buff[100];
      snprintf(buff, sizeof(buff), "%sgpio%d/edge", GPIO_PATH, pin);
      this->edgesPath = buff;
    }

    std::string getDirectionPath()
    {
      return this->directionPath;
    }

    std::string getValuePath()
    {
      return this->valuePath;
    }

  public:
    GPIO(int pin)
    {
      this->pin = pin;
      setDirectionPath();
      setValuePath();
      setEdgePath();
    }

    void setLEDValue(int state)
    {
      this->ledValue = state;
    }

    int getLEDValue()
    {
      return this->ledValue;
    }

    int writeDirection(std::string direction)
    {
      std::ofstream dir(getDirectionPath());
      if (!dir.is_open())
      {
        std::cerr << "Cannot open the file!\n";
        return 1;
      }

      dir << direction;
      dir.close();

      return 0;
    }

    int openGPIOValue()
    {
      this->ledFD = open(getValuePath().c_str(), O_RDWR);
      if (this->ledFD < 0)
      {
        std::cerr << "Cannot open the file!\n";
        return 1;
      }
      return ledFD;
    }

    void turnOnLED()
    {
      write(this->ledFD, "1", 1);
    }

    void turnOffLED()
    {
      write(this->ledFD, "0", 1);
    }

    int setEdgeGPIO(const char *edge)
    {
      char buf[50];
      int len = snprintf(buf, 50, edgesPath.c_str());
      int fd = open(buf, O_WRONLY);
      if (fd < 0)
      {
        printf("Cannot open the file!");
      }
      int writeEdge = write(fd, edge, strlen(edge) + 1);
      if (writeEdge < 0)
      {
        printf("Cannot write the file!");
      }
      close(fd);
      return 0;
    }

    // Keep this function for later
    void readFromGPIO(char *buf)
    {
      openGPIOValue();
      lseek(ledFD, 0, SEEK_SET);
      read(ledFD, buf, 4);
      close(ledFD);
    }
};
