#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include "string.h"
#include "GPIO.h"

#define GPIO_PATH "/sys/class/gpio/"

void GPIO::setDirectionPath()
{
  char buff[100];
  snprintf(buff, sizeof(buff), "%sgpio%d/direction", GPIO_PATH, pin);
  this->directionPath = buff;
}

void GPIO::setValuePath()
{
  char buff[100];
  snprintf(buff, sizeof(buff), "%sgpio%d/value", GPIO_PATH, pin);
  this->valuePath = buff;
}

void GPIO::setEdgePath()
{
  char buff[100];
  snprintf(buff, sizeof(buff), "%sgpio%d/edge", GPIO_PATH, pin);
  this->edgesPath = buff;
}

std::string GPIO::getDirectionPath()
{
  return this->directionPath;
}

std::string GPIO::getValuePath()
{
  return this->valuePath;
}


GPIO::GPIO(int pin)
{
  this->pin = pin;
  setDirectionPath();
  setValuePath();
  setEdgePath();
}

GPIO::~GPIO()
{
}

void GPIO::setLEDValue(int state)
{
  this->ledValue = state;
}

int GPIO::getLEDValue()
{
  return this->ledValue;
}

int GPIO::writeDirection(std::string direction)
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

int GPIO::openGPIOValue()
{
  this->ledFD = open(getValuePath().c_str(), O_RDWR);

  if (0 > this->ledFD)
  {
    std::cerr << "Cannot open the file!\n";
    return 1;
  }
  return ledFD;
}

void GPIO::turnOnLED()
{
  write(this->ledFD, "1", 1);
}

void GPIO::turnOffLED()
{
  write(this->ledFD, "0", 1);
}

int GPIO::setEdgeGPIO(const char *edge)
{
  char buf[50];
  int len = snprintf(buf, 50, edgesPath.c_str());
  int fd  = open(buf, O_WRONLY);

  if (0 > fd)
  {
    printf("Cannot open the file!");
  }

  int writeEdge = write(fd, edge, strlen(edge) + 1);

  if (0 > writeEdge)
  {
    printf("Cannot write the file!");
  }

  close(fd);
  return 0;
}

// Keep this function for later
void GPIO::readFromGPIO(char *buf)
{
  openGPIOValue();
  lseek(ledFD, 0, SEEK_SET);
  read(ledFD, buf, 4);
  close(ledFD);
}
