#include <iostream>

class GPIO
{
  private:
    std::string directionPath;
    std::string valuePath;
    std::string edgesPath;

    int pin;
    int ledFD;
    int ledValue;

    void setDirectionPath();
    void setValuePath();

    void setEdgePath();
    std::string getDirectionPath();
    std::string getValuePath();

  public:
    GPIO(int pin);
    ~GPIO();
    void setLEDValue(int state);
    int getLEDValue();
    int writeDirection(std::string direction);
    int openGPIOValue();
    void turnOnLED();
    void turnOffLED();
    int setEdgeGPIO(const char *edge);
    void readFromGPIO(char *buf);
};
