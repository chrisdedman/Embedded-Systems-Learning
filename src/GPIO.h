/* -----------------------------------------------------------------------------
  * GPIO.h
  * ----------------------------------------------------------------------------
  * Created on: January 2024
  * Author: Christopher Dedman-Rollet
  * ----------------------------------------------------------------------------
  * This file contains the GPIO class definition
  * 
  * The GPIO class is used to control the GPIO pins on the Raspvery Pi 3 B+ 
  * board using the sysfs interface provided by the Linux kernel to interact 
  * with the GPIO pins. The class provides methods to set the direction of the 
  * GPIO pin, set the value of the GPIO pin, read the value of the GPIO pin, and
  * set the edge of the GPIO pin. The class also provides methods to turn on and 
  * off an LED connected to the GPIO pin.
  * ----------------------------------------------------------------------------
*/
#include <iostream>

/**
 * @class GPIO
 * @brief Represents a GPIO configuration on the Raspberry Pi 3 B+ board.
 * 
 * This class provides functionality to control and interact with a GPIO pin.
 * It allows setting the pin direction, reading and writing the pin value,
 * and controlling an LED connected to the pin.
 */
class GPIO {
  private:
    std::string directionPath;    // The path to the pin direction file.
    std::string valuePath;        // The path to the pin value file.
    std::string edgesPath;        // The path to the pin edges file.

    int pin;                      // The pin number.
    int ledFD;                    // The file descriptor for the LED.
    int ledValue;                 // The value of the LED.

    /**
     * @brief Sets the direction path based on the pin number.
     */
    void setDirectionPath();

    /**
     * @brief Sets the value path based on the pin number.
     */
    void setValuePath();

    /**
     * @brief Sets the edge path based on the pin number.
     */
    void setEdgePath();

    /**
     * @brief Gets the direction path.
     * @return The path of the gpio direction file.
     */
    std::string getDirectionPath();

    /**
     * @brief Gets the value path.
     * @return The path of the gpio value file.
     */
    std::string getValuePath();

  public:
    /**
     * @brief Constructs a GPIO object with the specified pin number.
     * @param pin The gpio pin number to control.
     */
    GPIO(int pin);

    /**
     * @brief Destroys the GPIO object.
     */
    ~GPIO();

    /**
     * @brief Sets the value of the LED.
     * @param state The state to set the LED to (0 for off, 1 for on).
     */
    void setLEDValue(int state);

    /**
     * @brief Gets the value of the LED.
     * @return The value of the LED (0 for off, 1 for on).
     */
    int getLEDValue();

    /**
     * @brief Writes the pin direction.
     * @param direction The direction to write ("in" for input, "out" for output).
     * @return 0 if successful, -1 otherwise.
     */
    int writeDirection(std::string direction);

    /**
     * @brief Opens the GPIO value file.
     * @return 0 if successful, -1 otherwise.
     */
    int openGPIOValue();

    /**
     * @brief Turns on the LED.
     */
    void turnOnLED();

    /**
     * @brief Turns off the LED.
     */
    void turnOffLED();

    /**
     * @brief Sets the edge of the GPIO pin.
     * @param edge The edge to set ("none", "rising", "falling", "both").
     * @return 0 if successful, -1 otherwise.
     */
    int setEdgeGPIO(const char *edge);

    /**
     * @brief Reads from the GPIO pin.
     * @param buf The buffer to read into.
     */
    void readFromGPIO(char *buf);
};
