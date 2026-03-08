# Otto HP Lite 🤖

[![Platform: MicroPython](https://img.shields.io/badge/Platform-MicroPython-blue.svg)](https://micropython.org)
[![Platform: Arduino/PlatformIO](https://img.shields.io/badge/Platform-Arduino-orange.svg)](https://arduino.cc)

A lightweight, fast-loading library and reference guide for the **HP Otto Robot (ESP32)**. Designed for developers and hobbyists who want to avoid heavy official libraries, build faster tests, and focus on personal projects.

## 📖 Documentation
We've organized the documentation to help you find what you need quickly:
* [🔌 Electronics & Pinout Guide](docs/electronics.md) - GPIO mappings and board details.
* [📝 Code Examples Guide](docs/examples_guide.md) - Block-to-Python mappings and expansion kit tests.

## 🚀 Quick Start Setup

### For MicroPython (Recommended for quick testing)
1. Ensure your ESP32 is flashed with MicroPython.
2. Use [Thonny IDE](https://thonny.org/) or VSCode with the Pymakr extension.
3. Check the `examples/micropython/` folder for ready-to-use scripts.

### For C++ / PlatformIO
1. Install [VSCode](https://code.visualstudio.com/) and the **PlatformIO IDE** extension.
2. Create a New Project with Board: `ESP32 Dev Module` and Framework: `Arduino`.
3. Check the `examples/arduino/` folder for project structures.

## 💡 Quick Example (MicroPython)
Here is a minimal script to move Otto's motors:

```python
from machine import Pin, PWM
from time import sleep

# Connectors 10 & 11
left_servo = PWM(Pin(13), freq=50)
right_servo = PWM(Pin(33), freq=50)

# Move
left_servo.duty(100)
right_servo.duty(40)
sleep(1)

# Stop
left_servo.duty(0)
right_servo.duty(0)