#Electronic

## Electronic Schema
Attached the basic pinout of the main and only circuit board of the HP Robots Otto. For more technical details go to:

https://hprobots.com/otto-robot/product/#specifications

The circuit board has a total 11 connectors labeled with simple printed numbers on top of each female connector ("Connector #")

The first three (1, 2, 3) have 4 pins for a cable of 4 wires, all the rest have 3 pins for a cable of 3 wires.

This is easy to recognize in our web code words app when you need to switch between components by selecting a different number and moving the component to that connector.

But when in the text programming, the generated code in blocks or with Micropython or Arduino, the connector # is translated to the corresponding GPIO pin number according to the ESP chip real input/output signal numbers as following:

- Connector #1: GPIO 18 & 19 (ultrasonic or OLED)
- Connector #2: GPIO 16 & 17 (mp3 or encoder)
- Connector #3: GPIO 22 & 21 I2C SCL & SDA (matrix or accelerometer)
- Connector #4: GPIO 26 (tilt or button)
- Connector #5: GPIO 4 (LED ring or temperature sensor)
- Connector #6: GPIO 32 ADC good for analog inputs (line sensor left or microphone)
- Connector #7: GPIO 33 ADC good for analog inputs (line sensor right or potentiometer or light sensor)
- Connector #8: GPIO 27 (servo angle right)
- Connector #9: GPIO 15 (servo angle right)
- Connector #10: GPIO 13 (servo wheel left)
- Connector #11: GPIO 33 (servo wheel right)
In parenthesis we put the default wiring connections assigned to our robot kits, you can rearrange modularly most of them depending on your application.

In terms of integrated components in the board we have additional control of an LED, a buzzer and the battery voltage, make sure NOT to use this numbers in the code for other purposes.

Built in LED: GPIO 2
Built in Buzzer: GPIO 25
Battery voltage: GPIO 39

<img width="1013" height="974" alt="1732531013-Otto-circuitboardpinout" src="https://github.com/user-attachments/assets/a4aa598d-548c-4156-bb25-fa704880cfc2" />



# Setup enviroment
[Set up video Arduino IDE](https://www.youtube.com/watch?v=5ssD2PqnxxY)
---

## ✅ Requirements

### 💻 Software:

* Visual Studio Code
* - [PlatformIO extension](https://platformio.org/install/ide?install=vscode) (or Arduino extension for VSCode)
* - USB drivers (e.g., CH340 for Arduino Nano clones)
* Arduino IDE
* - [Ottos library](https://github.com/OttoDIY/OttoDIYLib)

---

## 🧭 Step-by-Step Setup with PlatformIO (Recommended)

### 1. **Install VSCode & PlatformIO**

* Open VSCode
* Go to Extensions (`Ctrl+Shift+X`)
* Search for **PlatformIO IDE** and install it

### 2. **Create a New Project**

* Open PlatformIO Home (Alien icon in the sidebar)
* Click “New Project”

  * Name: `OttoProject`
  * Board: Choose `ESP32 Dev Module` 
  * Framework: Arduino
  * Location: choose your folder

### 3. **Write or Paste Your Code**

* Your code goes into `src/main.cpp`

**Example:**

```cpp
import machine, time                       #importing machine and time libraries
from time import sleep                     #importing sleep class
from machine import Pin, ADC, PWM, SoftI2C #importing classes
from ottomotor import OttoMotor
offset = 0

offset = 0
motor = OttoMotor(13, 14)             # Connectors 10 & 11

motor.leftServo.freq(50)
motor.rightServo.freq(50)
motor.leftServo.duty(109- offset)
motor.rightServo.duty(43+ offset)
sleep(1)
motor.rightServo.duty(0)
motor.leftServo.duty(0)
motor.rightServo.duty(45+ offset)
motor.leftServo.duty(45- offset)
sleep(0.4)
motor.rightServo.duty(0)
motor.leftServo.duty(0)
motor.leftServo.freq(50)
motor.rightServo.freq(50)
motor.leftServo.duty(43- offset)
motor.rightServo.duty(109+ offset)
sleep(1)
motor.rightServo.duty(0)
motor.leftServo.duty(0)
motor.rightServo.duty(115+ offset)
motor.leftServo.duty(115- offset)
sleep(0.4)
motor.rightServo.duty(0)
motor.leftServo.duty(0)

```

> Make sure you have installed the Otto9 library (see below).

---

### 4. **Install Otto Library**

You can add Otto libraries using PlatformIO:

* Go to the **PlatformIO Library** tab
* Search for `Otto9`
* Click "Add to Project"

OR manually add it in `platformio.ini`:

```ini
[env:nanoatmega328]
platform = atmelavr
board = nanoatmega328
framework = arduino
lib_deps =
    OttoDIY/Otto9@^1.0.3
```

---

### 5. **Connect Otto via USB**

* Plug in Otto to your PC
* PlatformIO should auto-detect the serial port

If not, check:

* **Device Manager** (Windows): Look under COM ports
* **`ls /dev/tty.*`** (Mac): Look for `/dev/tty.usbserial-*` or similar

Add to `platformio.ini` if needed:

```ini
upload_port = COM3  ; Replace with your actual COM port
```

---

### 6. **Upload Your Code**

Click the **arrow icon** at the bottom bar of PlatformIO (or run `PlatformIO: Upload` from Command Palette)

✅ If everything is correct, the code will compile and upload to Otto.

---

## 🛠️ Troubleshooting

| Problem                  | Fix                                                                                 |
| ------------------------ | ----------------------------------------------------------------------------------- |
| "Board not found"        | Check COM port, drivers                                                             |
| “avrdude: stk500…” error | Wrong board or bootloader — try both `ATmega328P` and `ATmega328P (Old Bootloader)` |
| RGB/OLED not working     | Check for hardware conflicts as you noted                                           |

---

## 📎 Alternative: Arduino Extension in VSCode

You can also use the **Arduino extension** instead of PlatformIO:

* Install Arduino IDE (to get the tools and drivers)
* Install Arduino extension in VSCode
* Set up `arduino.json` and `c_cpp_properties.json`
* But: **less flexible than PlatformIO**, harder to manage libraries

---



# Code


Here’s a concise “Block-to-Python” reference guide that shows you:

1. **How to start a new Otto project**
2. **How to configure each hardware block**
3. **How to call them from your Python code**

---

## 1. Starting a New Project

1. **Create your script**

   ```bash
   # On your Otto’s MicroPython filesystem:
   /flash/main.py
   ```
2. **Import the core libraries** you’ll need (only import what you use):

   ```python
   import machine, time
   from time import sleep
   from machine import Pin, ADC, PWM, SoftI2C
   ```
3. **Import Otto blocks** for buzzer, motors, LEDs, sensors, etc.:

   ```python
   from ottobuzzer   import OttoBuzzer         # sound
   from ottomotor    import OttoMotor          # servos
   from ottoneopixel import OttoNeoPixel       # LED ring
   from ottoneopixel import OttoUltrasonic     # “eye” LEDs + ultrasonic
   from ottosensors  import DHT, RotaryIRQ     # sense-kit modules
   # …or NeoPixel/Player/Ninja modules as needed
   ```

---

## 2. Configuring Components

### 2.1 Buzzer

```python
buzzer = OttoBuzzer(pin)      # pin = e.g. 25
```

* **Play built-in emojis**:

  ```python
  buzzer.playEmoji("S_happy")
  ```
* **Play custom notes**:

  ```python
  buzzer.playNote(262, 125)  # (frequency, duration ms)
  ```

### 2.2 Motors (Servos)

```python
motor = OttoMotor(pinR, pinL)  # e.g. (13, 14)
```

* **Set PWM frequency** (optional):

  ```python
  motor.leftServo.freq(50)
  motor.rightServo.freq(50)
  ```
* **Direct duty (position/speed)**:

  ```python
  motor.leftServo.duty(95 - offset)
  motor.rightServo.duty(43 + offset)
  ```
* **High-level moves** (block-style helpers):

  ```python
  motor.Moveleft(speed, duration, steps)
  motor.Moveright(speed, duration, steps)
  motor.Stop()          # stop both servos
  ```

### 2.3 RGB Ring (NeoPixel)

```python
n    = 13
ring = OttoNeoPixel(pin, n)   # pin = e.g. 4
```

* **Global brightness**:

  ```python
  ring.setBrightness(0.5)    # 0–1.0
  ```
* **Fill patterns**:

  ```python
  ring.fillAllRGBRing("ff0000")           # all red
  ring.rainbow_cycle(n, speed)
  ring.bounce(n, r, g, b, delay)
  ring.cycle  (n, r, g, b, delay)
  ```
* **Per-LED control**:

  ```python
  ring.setRGBring(index, "00ff00")
  ```

### 2.4 “Eyes” & Ultrasonic

```python
ultrasonic = OttoUltrasonic(pinTr, pinEc)  # e.g. (18, 19)
```

* **Static eye colors**:

  ```python
  ultrasonic.ultrasonicRGB1("00ff00", "00ff00")
  ultrasonic.clearultrasonicRGB()
  ```
* **Read distance**:

  ```python
  dist = int(ultrasonic.readDistance())   # use readDistance(), not readultrasonicRGB
  ```

### 2.5 Sense-Kit Modules

* **Temperature & Humidity**:

  ```python
  from ottosensors import DHT
  d = DHT(pin)      # e.g. 26
  temp = d.temperature()
  hum  = d.humidity()
  ```
* **Photo-resistor (light)**:

  ```python
  adcL = ADC(Pin(33))
  adcL.width(ADC.WIDTH_12BIT)
  adcL.atten(ADC.ATTN_6DB)
  lux = map(adcL.read(), 0, 4095, 0, 100)
  ```
* **Microphone**: same as light, but map ADC32 → sound level.
* **Rotary Encoder**:

  ```python
  from ottosensors import RotaryIRQ
  enc = RotaryIRQ(clk, dt, min_val, max_val, reverse, range_mode)
  val = enc.value()
  ```
* **Accelerometer (Interact kit)**:

  ```python
  from adxl345 import ADXL345
  i2c = SoftI2C(scl=Pin(22), sda=Pin(21))
  accel = ADXL345(i2c)
  x,y,z = accel.ReadXYZ()
  ```

---

## 3. Putting It All Together

Below is a minimal “Test 1” template showing **sound → lights → motors → obstacle loop**:

```python
# 1) Initialize
buzzer    = OttoBuzzer(25)
motor     = OttoMotor(13, 14)
ring      = OttoNeoPixel(4, 13)
ultrasonic= OttoUltrasonic(18, 19)

# 2) Hardware tests
buzzer.playEmoji("S_connection")

ring.rainbow_cycle(13, 2)
ultrasonic.ultrasonicRGB1("00ff00","00ff00")

motor.Moveleft(-1,1,1)
motor.Moveright(-1,1,1)

# 3) Main loop: obstacle warning
while True:
    dist = int(ultrasonic.readDistance())
    if dist <= 10:
        ring.fillAllRGBRing("fe0000")
        buzzer.playEmoji("S_surprise")
    else:
        ring.fillAllRGBRing("33ff33")
```

---

## 4. Block → Python Mapping

| **Block**                        | **Python**                                     |
| -------------------------------- | ---------------------------------------------- |
| “Buzzer ► Emoji S\_happy”        | `buzzer.playEmoji("S_happy")`                  |
| “Buzzer ► Play note 262, 125”    | `buzzer.playNote(262, 125)`                    |
| “Ring ► Rainbow cycle speed 5”   | `ring.rainbow_cycle(13, 5)`                    |
| “Ring ► Fill all color ff0000”   | `ring.fillAllRGBRing("ff0000")`                |
| “Motor ► Move left speed, time…” | `motor.Moveleft(speed, duration, steps)`       |
| “Motor ► Stop”                   | `motor.Stop()`                                 |
| “Eyes ► Set colors 00ff00”       | `ultrasonic.ultrasonicRGB1("00ff00","00ff00")` |
| “Ultrasonic ► Read distance”     | `dist = int(ultrasonic.readDistance())`        |
| “If distance ≤ 10 cm ► do…”      | `if dist <= 10: … else: …`                     |
| “Analog read ► map 0–4095→0–100” | `level = map(adc.read(), 0,4095, 0,100)`       |
| “DHT ► Read temp / humidity”     | `temp = d.temperature(); hum = d.humidity()`   |
| “Rotary ► Read value”            | `val = encoder.value()`                        |

---

### 5. Quick Tips

* **Always instantiate** your sensors/actuators before calling their methods.
* **Map raw ADC** readings into a human range (0–100, 0–270°, etc.) using a small helper function.
* **Use `readDistance()`** (not the RGB-based read) to get a numeric distance.
* **Keep your main loop** clean: read, test, then call high-level block-style methods.
* **Organize** repeated sequences into your own Python functions for clarity.

Keep learning and expand Otto’s behaviors one block at a time. Happy coding!

---


---

Now, here's a set of **simple and self-contained test examples** using each of the **expansion kit components**: **Sense**, **Interact**, and **Emote**.


## ✅ Expansion pack: **Sense**

These are the sensors — like light, distance, and sound.

---

### 🔹 **Test 1: Light Sensor**

**Goal:** Print brightness value from the light sensor every second.

```python
from hummingbird import *
import time

while True:
    brightness = get_light_sensor(1)
    print("Brightness:", brightness)
    time.sleep(1)
```

---

### 🔹 **Test 2: Distance Sensor**

**Goal:** Print distance in cm, stop if object is too close.

```python
from hummingbird import *
import time

while True:
    distance = get_distance_sensor(1)
    print("Distance (cm):", distance)
    if distance < 10:
        print("Too close! Stopping.")
        break
    time.sleep(0.5)
```

---

### 🔹 **Test 3: Sound Sensor**

**Goal:** Print sound level if above a threshold (e.g., loud clap).

```python
from hummingbird import *
import time

while True:
    sound = get_sound_sensor(1)
    if sound > 50:
        print("Loud sound detected:", sound)
    time.sleep(0.2)
```

---

## ✅ Expansion pack: **Interact**

These are inputs: **buttons**, **knobs**, and **sliders**.

---

### 🔹 **Test 4: Rotary Knob (Potentiometer)**

**Goal:** Read knob position and print it.

```python
from hummingbird import *
import time

while True:
    value = get_knob_value(1)
    print("Knob position:", value)
    time.sleep(0.5)
```

---

### 🔹 **Test 5: Push Button**

**Goal:** Print message when button is pressed.

```python
from hummingbird import *
import time

while True:
    if get_button_state(1):
        print("Button is pressed!")
    else:
        print("Button not pressed.")
    time.sleep(0.5)
```

---

## ✅ Expansion pack: **Emote**

These are the expressive output components: **tri-color LED**, **position servo**, and **vibration motor**.

---

### 🔹 **Test 6: Tri-color LED**

**Goal:** Cycle through Red, Green, and Blue every second.

```python
from hummingbird import *
import time

while True:
    set_tricolor_led(1, 255, 0, 0)  # Red
    time.sleep(1)
    set_tricolor_led(1, 0, 255, 0)  # Green
    time.sleep(1)
    set_tricolor_led(1, 0, 0, 255)  # Blue
    time.sleep(1)
```

---

### 🔹 **Test 7: Position Servo**

**Goal:** Sweep the servo from 0° to 180° and back.

```python
from hummingbird import *
import time

while True:
    for angle in range(0, 181, 30):
        set_position_servo(1, angle)
        time.sleep(0.3)
    for angle in range(180, -1, -30):
        set_position_servo(1, angle)
        time.sleep(0.3)
```

---

### 🔹 **Test 8: Vibration Motor**

**Goal:** Pulse the vibration motor on and off.

```python
from hummingbird import *
import time

while True:
    set_vibration_motor(1, 100)  # Full power
    time.sleep(0.5)
    set_vibration_motor(1, 0)
    time.sleep(0.5)
```

---

Let me know if you'd like to combine these into interactive projects or add graphical or logic-based controls!
