# Gesture Interaction Robot (GIR) 🤖

A computer vision and embedded systems project focused on creating an interactive robot capable of understanding human gestures, responding through motion and sound, and performing autonomous behaviors.

GIR combines **real-time computer vision, embedded control, servo-based motion systems, and human–robot interaction** to create a responsive robotic platform controlled through hand gestures, voice feedback, and environmental inputs.

---

## 📌 Project Overview

The GIR system uses a webcam to capture human hand movements and processes them using **Python, OpenCV, and MediaPipe** for real-time gesture recognition.

Recognized gestures are converted into high-level commands and transmitted through serial communication to an **Arduino-based control system**.

The Arduino manages the robot's physical behaviors, including:

- Hand movement
- Head rotation
- Body movement
- Eye interaction
- Audio feedback
- System state management

The robot also supports **double-clap detection** for power control and interactive behaviors.

---

# ✨ Features

## 👋 Computer Vision & Gesture Recognition

- Real-time hand tracking using MediaPipe
- Left and right hand detection
- Finger counting-based command recognition
- Gesture-to-command mapping
- Fail-safe operation when no gesture is detected

## 🤖 Robotic Motion Control

- Smooth servo movements using motion easing techniques
- Multi-servo coordination
- Human-like movement patterns
- Control of:
  - Hands
  - Head
  - Body rotation
  - Eyes

## 🔊 Audio Interaction

- Voice and sound playback
- Audio feedback using DFPlayer Mini
- Microphone-based clap detection
- Double-clap activation system

## 🔌 Communication System

- Python-to-Arduino serial communication
- USB / Bluetooth communication support
- Real-time command transmission

---

# 🛠️ Technologies Used

## Software

| Technology | Purpose |
|---|---|
| Python | Main processing and control logic |
| OpenCV | Computer vision processing |
| MediaPipe | Hand tracking and gesture recognition |
| PySerial | Communication with Arduino |

## Hardware

| Component | Purpose |
|---|---|
| Arduino Uno R3 | Main embedded controller |
| Servo Motors | Robot movement |
| DFPlayer Mini | Audio playback |
| Microphone Sensor | Clap detection |
| EEPROM | Memory and state storage |

---

# 🎮 Gesture Control Mapping

| Gesture | Robot Action |
|---|---|
| Right hand raised | Control right hand movement |
| Left hand raised | Control left hand movement |
| Index finger movement | Control head direction |
| Thumb + Index gesture | Rotate robot body |
| Double clap | Turn robot system ON/OFF |

---

# ⚙️ System Architecture

```
                 Human Interaction
                         |
          ┌──────────────┴──────────────┐
          |                             |
       Webcam                     Voice / Sound
          |                             |
          ↓                             ↓
   Python + OpenCV              Audio Processing
   + MediaPipe
          |
          ↓
 Gesture Recognition
          |
          ↓
 Command Generation
          |
          ↓
 Serial Communication
          |
          ↓
 Arduino Controller
          |
 ┌────────┼────────┐
 ↓        ↓        ↓
Servo   Audio   Sensors
System  System  System
```
# 👨‍💻 Developer
**Unes Jami**
Robotics • Embedded Systems • Artificial Intelligence

# 📜 License
This project is licensed under the MIT License.
