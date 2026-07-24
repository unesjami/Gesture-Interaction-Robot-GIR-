# Gesture-Interaction-Robot-GIR-
A computer vision and embedded systems project featuring real-time hand gesture recognition, smooth servo motion, voice feedback, and autonomous robot behaviors.
An interactive humanoid robot controlled by hand gestures, sound, and sensors, combining computer vision, embedded systems, and human–robot interaction.
This project uses Python + OpenCV + MediaPipe for real-time gesture recognition and an Arduino for smooth servo control, audio feedback, and autonomous behaviors.

🔹 Project Overview
The system detects human hand gestures using a webcam and translates them into high-level commands.
These commands are sent to an Arduino, which controls the robot’s hands, head, body, eyes, and audio responses.
The robot can also turn on/off by double clap.

🧠 Features
✋ Real-time hand gesture recognition (left & right hand detection)
🧮 Finger counting–based control logic/serial commands
🦾 Smooth, human-like servo movements using easing functions
🎧 Voice & sound playback via DFPlayer Mini
👀 Eye control + turn on/off with double clap 👏 Double-clap detection to trigger an introduction animation
🤖 Head, body, and arm control through intuitive gestures
🔄 Failsafe behavior when no hand is detected

🛠 Technologies Used
Software: Python, OpenCV, MediaPipe, PySerial. Hardware: Arduino Uno R3, Servo Motors (Hands, Head, Body), DFPlayer Mini (Audio module), Microphone Sensor (Clap detection), EEPROM (State memory)

🎮 Gesture Controls
Gesture Action
Right Hand = Raise / Lower right hand
Left Hand = Raise / Lower left hand
Right Hand + Index finger movement = Control robot head direction
Right Hand + Thumb + Index = Rotate robot body
Double clap turn off/on robot

🧩 System Architecture
Webcam captures live video
Python processes frames using MediaPipe
Gestures are converted to text commands
Commands are sent via Serial (Bluetooth / USB)
Arduino executes motion, sound, and sensor logic
