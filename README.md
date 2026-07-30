# 🤖 Gesture Interaction Robot (GIR)

> **A computer vision and embedded systems project featuring real-time hand gesture recognition, smooth servo motion, voice feedback, and autonomous robot behaviors.**

GIR is an interactive humanoid robot that combines **computer vision, embedded systems, servo control, audio feedback, and human–robot interaction**.

The system uses a webcam to recognize hand gestures with **Python, OpenCV, and MediaPipe**, then sends high-level commands to an **Arduino** for real-time robot movement and behavior.

---

## ✨ Features

* 🖐️ Real-time left and right hand gesture recognition
* 👆 Finger-count-based gesture control
* 🤖 Head, body, and arm movement
* ⚙️ Smooth servo motion with easing
* 🔊 Voice and sound feedback using DFPlayer Mini
* 👀 Robot eye control
* 👏 Double-clap detection
* 💾 EEPROM-based state memory
* 🛡️ Failsafe behavior when no hand is detected
* 🔌 Serial communication between Python and Arduino

---

## 🧠 System Architecture

```text
             ┌──────────────────┐
             │      Webcam      │
             └────────┬─────────┘
                      │
                      ▼
             ┌──────────────────┐
             │ Python + OpenCV  │
             │    MediaPipe     │
             └────────┬─────────┘
                      │
               Gesture Commands
                      │
                      ▼
             ┌──────────────────┐
             │ Serial / USB /   │
             │    Bluetooth     │
             └────────┬─────────┘
                      │
                      ▼
             ┌──────────────────┐
             │   Arduino Uno    │
             └────────┬─────────┘
                      │
          ┌───────────┼───────────┐
          ▼           ▼           ▼
       Servos       Audio       Sensors
       & Eyes     DFPlayer      & State
```

---

## 🎮 Gesture Controls

| Gesture                    | Action                                |
| -------------------------- | ------------------------------------- |
| Right hand                 | Raise / lower right hand              |
| Left hand                  | Raise / lower left hand               |
| Right hand + index         | Control head direction                |
| Right hand + thumb + index | Rotate robot body                     |
| Double clap                | Turn robot on/off or trigger behavior |

---

## 🛠️ Technologies

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
* Robot eye system

---

## 📁 Project Structure

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

## 🚀 Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/unesjami/Gesture-Interaction-Robot-GIR-.git
cd Gesture-Interaction-Robot-GIR-
```

### 2. Install Python dependencies

```bash
pip install -r requirements.txt
```

### 3. Connect the hardware

Connect the Arduino and robot hardware according to the schematic located in:

```text
hardware/gir_schematic.jpg
```

### 4. Configure the serial connection

Open:

```text
src/python/gir_controller.py
```

Update the Arduino serial port if necessary:

```python
arduino = serial.Serial('COM5', 9600)
```

### 5. Run the Python controller

```bash
python src/python/gir_controller.py
```

---

## 📸 Project Media

Project photos and additional media are organized inside:

```text
media/
```

See [`media/README.md`](media/README.md) for details.

---

## 📐 Hardware Schematic

The current robot circuit schematic is available here:

[`hardware/gir_schematic.jpg`](hardware/gir_schematic.jpg)

---

## 🎤 Presentation

A project presentation is available at:

[`docs/presentation.html`](docs/presentation.html)

Open the HTML file in a browser to view the presentation.

---

## 🎯 Project Goals

GIR was developed to explore the integration of:

* Computer vision
* Human–robot interaction
* Embedded systems
* Servo control
* Gesture recognition
* Audio feedback
* Sensor-based interaction

The project demonstrates how software and hardware can work together to create a more natural method of interacting with robots.

---

## 📄 License

This project is licensed under the **MIT License**.

See [`LICENSE`](LICENSE) for details.
