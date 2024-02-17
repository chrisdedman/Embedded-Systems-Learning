#!/bin/bash

# Define an array of GPIO pin numbers
GPIO_PINS=(6 15 16 23 24)

# Function to export GPIO pins
export_gpio_pins() {
    for pin in "${GPIO_PINS[@]}"; do
        echo $pin > /sys/class/gpio/export
    done
    echo "GPIO PIN(s) now Open."
}

export_gpio_pins
