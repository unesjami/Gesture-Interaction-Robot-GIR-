# Gesture Interaction Robot (GIR)

**Gesture Interaction Robot (GIR)** is an interactive humanoid robot that combines **computer vision, embedded systems, gesture recognition, servo control, and audio feedback** to create a human–robot interaction experience.

The robot interprets hand gestures through a webcam and sends high-level commands from Python to an Arduino controller. The Arduino then coordinates the robot's movements, sounds, eyes, and other behaviors.

> **GIR — Gesture-controlled interaction between humans and robots.**

---

## Features

* Real-time hand gesture recognition
* Left and right hand detection
* Finger-counting gesture control
* Head direction control
* Body rotation control
* Independent right and left arm control
* Smooth servo movement using easing functions
* Voice and sound feedback using DFPlayer Mini
* Double-clap detection
* Robot activation and deactivation
* Autonomous fail-safe behavior
* Serial communication between Python and Arduino

---

## System Architecture

```text
                Webcam
                   │
                   ▼
        ┌─────────────────────┐
        │   Python Controller │
        │                     │
        │ OpenCV + MediaPipe  │
        │ Gesture Recognition  │
        └──────────┬──────────┘
                   │
             Serial Commands
                   │
                   ▼
        ┌─────────────────────┐
        │   Arduino Controller│
        │                     │
        │ Servo Control       │
        │ Audio / DFPlayer    │
        │ Sensors             │
        │ Robot Behaviors     │
        └──────────┬──────────┘
                   │
                   ▼
             GIR Robot
```

---

## Gesture Controls

| Gesture                    | Action                 |
| -------------------------- | ---------------------- |
| Right hand — 4 fingers     | Raise right hand       |
| Right hand lowered         | Lower right hand       |
| Left hand — 4 fingers      | Raise left hand        |
| Left hand lowered          | Lower left hand        |
| Right hand + index finger  | Control head direction |
| Right hand + thumb + index | Control body direction |
| Double clap                | Toggle robot state     |

---

## Technologies

### Software

* Python
* OpenCV
* MediaPipe
* PySerial

### Hardware

* Arduino Uno R3
* Servo motors
* DFPlayer Mini
* Microphone sensor
* EEPROM
* Robot LEDs / eyes
* Webcam

---

## Project Structure

```text
GIR/
│
├── README.md
├── LICENSE
├── .gitignore
├── requirements.txt
│
├── src/
│   ├── python/
│   │   └── gir_controller.py
│   │
│   └── arduino/
│       └── gir_arduino.ino
│
├── models/
│   └── hand_landmarker.task
│
├── assets/
│   ├── voice/
│   │   ├── 0001.mp3
│   │   ├── 0002.mp3
│   │   └── ...
│   │
│   └── images/
│
├── hardware/
│   └── gir_schematic.jpg
│
├── docs/
│   └── presentation.html
│
└── media/
    └── README.md
```

---

## How It Works

### 1. Gesture Detection

The webcam captures live video frames.

### 2. Computer Vision

MediaPipe detects hand landmarks while OpenCV handles image processing and visualization.

### 3. Gesture Interpretation

The Python controller analyzes the detected landmarks and identifies gestures such as:

* Hand raising
* Hand lowering
* Head movement
* Body movement

### 4. Serial Communication

Python converts the detected gesture into a command and sends it to the Arduino through serial communication.

### 5. Robot Control

The Arduino receives the command and controls:

* Servos
* Head
* Arms
* Body
* Eyes
* Audio
* Robot state

---

## Installation

Clone the repository:

```bash
git clone https://github.com/unesjami/Gesture-Interaction-Robot-GIR-.git
cd Gesture-Interaction-Robot-GIR-
```

Install the Python dependencies:

```bash
pip install -r requirements.txt
```

Make sure the required hardware is connected and the Arduino firmware is uploaded.

Then run:

```bash
python src/python/gir_controller.py
```

> **Note:** The serial port in `gir_controller.py` may need to be changed depending on your computer and Bluetooth/USB configuration.

---

## Hardware

The project includes the robot's circuit schematic:

`hardware/gir_schematic.jpg`

The hardware combines servo motors, an Arduino controller, audio playback, sensors, and other electronic components.

---

## Media

Project demonstrations, photos, and videos are organized separately from the source code.

See:

`media/README.md`

---

## Project Documentation

The project presentation is available here:

`docs/presentation.html`

You can open it directly in a web browser.

---

## Project Status

**Current status:** Working prototype

The GIR prototype demonstrates real-time gesture recognition, robot movement control, audio interaction, and sensor-based behavior.

Future improvements may include:

* More advanced gesture recognition
* Better motion planning
* Additional sensors
* Improved human–robot interaction
* Wireless communication
* More autonomous behaviors
* Expanded voice interaction

---

## License

This project is licensed under the **MIT License**.

See [`LICENSE`](LICENSE) for details.

---

## Author

**Unes Jami**

Electronics & Mechatronics Engineering
Computer Vision • Embedded Systems • Robotics • Automation

---

> **Gesture Interaction Robot (GIR)**
> *Bridging computer vision, embedded systems, and human–robot interaction.*
