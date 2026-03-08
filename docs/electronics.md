# HP Otto - Electronics & Pinout ⚡

Attached is the basic pinout of the main (and only) circuit board of the HP Robots Otto. For more official technical details, visit the [HP Robots Specifications page](https://hprobots.com/otto-robot/product/#specifications).

## The Circuit Board
The circuit board has a total of 11 connectors labeled with simple printed numbers on top of each female connector ("Connector #").
* **Connectors 1, 2, and 3** have 4 pins (for 4-wire cables).
* **Connectors 4 through 11** have 3 pins (for 3-wire cables).

While visual block-programming uses the connector numbers, text-based programming (MicroPython, C++) requires the actual **ESP32 GPIO pin numbers**. 

### Connector to GPIO Mapping

| Connector # | GPIO Pins | Recommended Component (Default Wiring) |
| :---: | :--- | :--- |
| **1** | 18, 19 | Ultrasonic or OLED |
| **2** | 16, 17 | MP3 or Encoder |
| **3** | 22, 21 | I2C SCL & SDA (Matrix or Accelerometer) |
| **4** | 26 | Tilt or Button |
| **5** | 4 | LED ring or Temperature sensor |
| **6** | 32 (ADC) | Analog inputs (Line sensor left or Microphone) |
| **7** | 33 (ADC) | Analog inputs (Line sensor right, Potentiometer, or Light sensor) |
| **8** | 27 | Servo angle right |
| **9** | 15 | Servo angle left |
| **10** | 14 | Servo wheel left |
| **11** | 13 | Servo wheel right |
| **Led** | 2 | Built in LED |
| **Buzzer** | 25 | Buzzer |
| **Battery** | 39 | Battery |

> **Note:** You can rearrange most of these modularly depending on your application.

---

## Built-in Components
The board features integrated components. **Do NOT use these GPIO numbers for external modules** to avoid conflicts:

* **Built-in LED:** GPIO 2
* **Built-in Buzzer:** GPIO 25
* **Battery Voltage Monitor:** GPIO 39

## Visual Reference
<img width="800" alt="Otto Circuit Board Pinout" src="https://github.com/user-attachments/assets/a4aa598d-548c-4156-bb25-fa704880cfc2" />