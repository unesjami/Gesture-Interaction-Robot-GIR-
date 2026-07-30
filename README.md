\# Gesture Interaction Robot (GIR)



\*\*Gesture-controlled humanoid robot combining computer vision, embedded systems, and human–robot interaction.\*\*



GIR is an interactive humanoid robot that interprets human hand gestures through a webcam and converts them into commands for an Arduino-based control system.



The project combines \*\*Python, OpenCV, MediaPipe, Arduino, servo motors, sensors, and audio feedback\*\* to create natural and interactive robot behavior.



\## Features



\* Real-time hand and gesture recognition

\* Left- and right-hand detection

\* Finger-counting-based gesture control

\* Head direction control

\* Robot body rotation control

\* Individual arm and hand movement

\* Smooth servo movement and state management

\* Voice and sound feedback using DFPlayer Mini

\* Double-clap interaction

\* Failsafe behavior when no hand is detected

\* Serial communication between computer vision and embedded control



\## How It Works



```text

Webcam

&#x20;  │

&#x20;  ▼

Python + OpenCV

&#x20;  │

&#x20;  ▼

MediaPipe Hand Tracking

&#x20;  │

&#x20;  ▼

Gesture Recognition

&#x20;  │

&#x20;  ▼

Serial Commands

&#x20;  │

&#x20;  ▼

Arduino

&#x20;  │

&#x20;  ├── Servo Motors

&#x20;  ├── Audio System

&#x20;  ├── Sensors

&#x20;  └── Robot Behaviors

```



The Python application captures live video from a webcam, detects hand landmarks with MediaPipe, interprets gestures, and sends high-level commands to the Arduino.



The Arduino receives these commands and controls the robot's physical movements, audio responses, and other behaviors.



\## Gesture Controls



| Gesture / Input            | Action                                     |

| -------------------------- | ------------------------------------------ |

| Right hand                 | Raise / lower right hand                   |

| Left hand                  | Raise / lower left hand                    |

| Right hand + index finger  | Control head direction                     |

| Right hand + thumb + index | Control body direction                     |

| Double clap                | Trigger robot power / interaction behavior |

| No detected hand           | Return to safe/default state               |



\## Technologies



\### Software



\* Python

\* OpenCV

\* MediaPipe

\* PySerial



\### Hardware



\* Arduino Uno R3

\* Servo motors

\* DFPlayer Mini

\* Microphone sensor

\* EEPROM

\* Webcam

\* Robot mechanical structure



\## Project Structure



```text

GIR/

├── assets/

│   └── voice/              # Robot audio files

├── docs/

│   └── presentation.html   # Project presentation

├── hardware/

│   └── gir\_schematic.jpg   # Circuit schematic

├── media/

│   └── README.md           # Media documentation

├── models/

│   └── hand\_landmarker.task

├── src/

│   ├── arduino/

│   │   └── gir\_arduino.ino

│   └── python/

│       └── gir\_controller.py

├── .gitignore

├── requirements.txt

├── LICENSE

└── README.md

```



\## Installation



\### 1. Clone the repository



```bash

git clone https://github.com/YOUR\_USERNAME/GIR.git

cd GIR

```



\### 2. Install Python dependencies



Python 3.10+ is recommended.



```bash

pip install -r requirements.txt

```



\### 3. Connect the hardware



Connect the Arduino to the computer and configure the appropriate serial connection.



The current Python controller uses:



```python

arduino = serial.Serial('COM5', 9600)

```



Change `COM5` to the serial port assigned to your Arduino.



\### 4. Run the computer-vision controller



```bash

python src/python/gir\_controller.py

```



A webcam is required for real-time hand tracking.



\## Arduino



Upload:



```text

src/arduino/gir\_arduino.ino

```



to the Arduino board using the Arduino IDE.



The Arduino firmware receives commands from the Python application and controls the robot's actuators and peripherals.



\## Hardware Schematic



The current circuit schematic is available in:



```text

hardware/gir\_schematic.jpg

```



\## Presentation



The project presentation is available at:



```text

docs/presentation.html

```



Open the HTML file in a modern web browser to view it.



\## Project Status



\*\*Prototype / Development\*\*



GIR is an ongoing robotics project. The current implementation focuses on gesture recognition, embedded control, servo movement, audio interaction, and human–robot interaction.



\## Future Improvements



\* Improve gesture classification accuracy

\* Add more natural gesture commands

\* Improve autonomous behaviors

\* Add wireless communication

\* Improve modularity of the Python controller

\* Add configuration files for hardware settings

\* Improve documentation and hardware diagrams

\* Add a dedicated GUI for robot control and monitoring



\## License



This project is licensed under the MIT License. See the `LICENSE` file for details.



\---



\*\*Gesture Interaction Robot (GIR)\*\*

Computer Vision • Embedded Systems • Robotics • Human–Robot Interaction



