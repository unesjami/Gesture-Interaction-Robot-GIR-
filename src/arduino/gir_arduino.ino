#include <Servo.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <EEPROM.h>
#include <math.h>

// ====== سروو موتورها ======
Servo servo1;  // دست راست
Servo servo2;  // دست چپ
Servo servo3;  // سر
Servo servo4;  // بدن

// Starting positions (OFF positions - all servos relaxed)
int right = 0;
int left = 180;
int head = 90;
int body = 90;

// Store current positions
int currentRight = 0;
int currentLeft = 180;
int currentHead = 90;
int currentBody = 90;

String headState = "CENTER";
String bodyState = "BODY_NONE";

bool servoBusy = false;
bool rightPlayed = false;
bool leftPlayed = false;
bool headPlayed = false;

// ====== DFPlayer Mini ======
SoftwareSerial mySerial(7, 8); // RX, TX
DFRobotDFPlayerMini dfplayer;

// ====== کنترل بدن ======
bool bodyAttached = false;
bool bodyActive = false; // ADDED: Track if body movement is active
unsigned long previousMillis = 0;
const int speedDelay = 15;

// ====== چشم‌ها (IR Sensor) ======
const int sensorPin = 2;
const int eyesPin = 5;
int lastSensorState = LOW;
bool eyesOn = false;
const int EEPROM_ADDR = 0;

// ====== میکروفون ======
const int micPin = 3;
int lastMicState = LOW;
int clapCount = 0;
unsigned long firstClapTime = 0;
unsigned long lastClapDetectionTime = 0;
const unsigned long clapWindow = 500;
const unsigned long debounceTime = 50;
bool introducing = false;
unsigned long introStartTime = 0;
const unsigned long introDuration = 6000;

// ====== سروو کنترل ======
bool servosEnabled = false; // ADDED: Control servo power state

// ====== EYE ANIMATION FEATURES ======
unsigned long lastEyeUpdate = 0;
unsigned long eyeAnimationStart = 0;
bool eyeAnimationActive = false;
int eyeAnimationType = 0; // 0: idle breathing, 1: speaking
int currentEyeBrightness = 0;
int targetEyeBrightness = 0;
float eyeAnimationProgress = 0;
const int BREATHING_CYCLE = 4000; // 4 seconds for full breathing cycle
const int SPEAKING_CYCLE = 800;   // 800ms for speaking animation

// ====== AUDIO CONTROL ======
unsigned long lastAudioTime = 0;
const unsigned long audioCooldown = 2000; // 2 second cooldown between audio plays
bool audioPlaying = false;
String lastCommand = "";
unsigned long audioStartTime = 0;
bool audioJustStarted = false;

// ====== AUTO-SHUTDOWN FEATURES ======
unsigned long lastActivityTime = 0;
const unsigned long shutdownTimeout = 180000; // 60 seconds auto-shutdown
bool shutdownWarningPlayed = false;

// =====================================================
// ====== Setup ======
void setup() {
  Serial.begin(9600);

  
  mySerial.begin(9600);

  if (!dfplayer.begin(mySerial)) {
    Serial.println("DFPlayer not connected!");
    while (true);
  }
  dfplayer.volume(25);

  // IMPORTANT: Initially DETACH all servos to prevent noise/movement
  // They will be attached only when needed
  
  pinMode(sensorPin, INPUT);
  pinMode(eyesPin, OUTPUT);
  pinMode(micPin, INPUT);

  // Start robot OFF on power-up
  eyesOn = false;
  fadeEyesOff();
  servosEnabled = false; // Keep servos disabled initially
  EEPROM.update(EEPROM_ADDR, 0);
  
  // Initialize activity timer
  lastActivityTime = millis();

  Serial.println("System Ready... Robot is OFF (Double clap to turn on)");
}

// =====================================================
// ====== Main Loop ======
void loop() {
  // Handle intro animation if active
  if (introducing) {
    handleIntroAnimation();
    return;
  }

  handleIRSensor();
  handleClapDetection();
  handleSerialCommands();
  handleHeadMovement();
  
  // Handle eye animations (always runs)
  handleEyeAnimations();
  
  // Handle auto-shutdown
  handleAutoShutdown();
}

// =====================================================
// ====== Functions ======

// Smooth fade eyes ON
void fadeEyesOn() {
  for (int i = 0; i <= 255; i += 5) {
    analogWrite(eyesPin, i);
    delay(10);
  }
  currentEyeBrightness = 255;
}

// Smooth fade eyes OFF
void fadeEyesOff() {
  for (int i = 255; i >= 0; i -= 5) {
    analogWrite(eyesPin, i);
    delay(10);
  }
  currentEyeBrightness = 0;
}

// Handle IR sensor toggle
void handleIRSensor() {
  int sensorState = digitalRead(sensorPin);

  if (sensorState == HIGH && lastSensorState == LOW) {
    eyesOn = !eyesOn;

    if (eyesOn) {
      dfplayer.play(9);      // Eyes ON sound
      fadeEyesOn();          
      startEyeAnimation(0); // Start idle breathing
      resetActivityTimer(); // Reset auto-shutdown timer
    } else {
      dfplayer.play(8);      // Eyes OFF sound
      fadeEyesOff();         
      stopEyeAnimation();
    }

    EEPROM.update(EEPROM_ADDR, eyesOn ? 1 : 0);
    Serial.print("Eyes: ");
    Serial.println(eyesOn ? "ON" : "OFF");
    delay(300); // debounce
  }
  lastSensorState = sensorState;
}

// CORRECTED CLAP DETECTION FUNCTION
void handleClapDetection() {
  int micState = digitalRead(micPin);
  unsigned long now = millis();

  // Detect rising edge (sound detected)
  if (micState == HIGH && lastMicState == LOW) {
    // Debounce: ignore if too close to previous detection
    if (now - lastClapDetectionTime < debounceTime) {
      lastMicState = micState;
      return;
    }
    
    lastClapDetectionTime = now; // Update for debouncing
    
    // Handle clap counting
    if (clapCount == 0) {
      // First clap detected
      clapCount = 1;
      firstClapTime = now;
    } 
    else if (clapCount == 1) {
      // Check if second clap is within time window
      if ((now - firstClapTime) <= clapWindow) {
        clapCount = 2; // Valid double clap
      } else {
        // Too late, restart with this clap as first
        clapCount = 1;
        firstClapTime = now;
      }
    }
    else {
      // If somehow clapCount is > 1, reset it
      clapCount = 1;
      firstClapTime = now;
    }
  }

  lastMicState = micState;

  // Check if double clap occurred
  if (clapCount == 2) {
    // Toggle robot state
    eyesOn = !eyesOn;

    if (eyesOn) {
      Serial.println("👏 Double clap detected — robot ON");
      fadeEyesOn();
      dfplayer.play(9); // Eyes ON sound
      // Do NOT move servos when turning on - they stay detached
      servosEnabled = true; // Enable servo control
      startEyeAnimation(0); // Start idle breathing
      resetActivityTimer(); // Reset auto-shutdown timer
    } else {
      Serial.println("👏 Double clap detected — robot OFF");
      fadeEyesOff();
      dfplayer.play(8); // Eyes OFF sound
      // Detach all servos when turning off
      detachAllServos();
      servosEnabled = false; // Disable servo control
      stopEyeAnimation();
    }

    // Reset for next detection
    clapCount = 0;
    delay(300); // Prevent immediate re-trigger
  }

  // Reset clapCount if too much time has passed without second clap
  if (clapCount == 1 && (now - firstClapTime) > clapWindow) {
    clapCount = 0;
  }
}

// Detach all servos (for turning off)
void detachAllServos() {
  if (servo1.attached()) servo1.detach();
  if (servo2.attached()) servo2.detach();
  if (servo3.attached()) servo3.detach();
  if (servo4.attached()) servo4.detach();
  bodyAttached = false;
  Serial.println("All servos detached (Robot OFF)");
}

// Handle serial commands
void handleSerialCommands() {
  if (!Serial.available()) return;

  String command = Serial.readStringUntil('\n');
  command.trim();
  
  // Check for ON/OFF commands (case insensitive)
  if (command.equalsIgnoreCase("on") || command.equalsIgnoreCase("robot on")) {
    if (!eyesOn) {
      Serial.println("🔌 Serial command: Robot ON");
      eyesOn = true;
      fadeEyesOn();
      dfplayer.play(9); // Eyes ON sound
      servosEnabled = true; // Enable servo control
      startEyeAnimation(0); // Start idle breathing
      resetActivityTimer(); // Reset auto-shutdown timer
      EEPROM.update(EEPROM_ADDR, 1);
      Serial.println("🤖 Robot is now ON");
    } else {
      Serial.println("🤖 Robot is already ON");
    }
    return;
  }
  
  if (command.equalsIgnoreCase("off") || command.equalsIgnoreCase("robot off")) {
    if (eyesOn) {
      Serial.println("🔌 Serial command: Robot OFF");
      eyesOn = false;
      fadeEyesOff();
      dfplayer.play(8); // Eyes OFF sound
      detachAllServos();
      servosEnabled = false; // Disable servo control
      stopEyeAnimation();
      EEPROM.update(EEPROM_ADDR, 0);
      Serial.println("🤖 Robot is now OFF");
    } else {
      Serial.println("🤖 Robot is already OFF");
    }
    return;
  }
  
  // Process other commands (original functionality)
  command.toLowerCase();
  if (command.length() == 0) return;

  // Check if servos are enabled (robot is ON)
  if (!servosEnabled) {
    Serial.println("Robot is OFF. Send 'on' or 'robot on' command or double clap to turn on.");
    return;
  }

  // Check if this is the same command as last time (ignore repeats)
  if (command == lastCommand && (millis() - lastAudioTime) < 1000) {
    return; // Ignore same command within 1 second
  }

  // Update last command
  lastCommand = command;

  // Reset activity timer for any command
  resetActivityTimer();

  // Stop any current body movement before new command
  if (bodyActive) {
    detachBodyServo();
    bodyActive = false;
  }

  // Check audio cooldown
  if ((millis() - lastAudioTime) < audioCooldown) {
    return; // Still in cooldown period
  }

  if (!servoBusy) {
    servoBusy = true; // Set busy flag first
    
    // RIGHT HAND COMMANDS
    if (command == "raise your right hand" && !rightPlayed) {
      // Detach other servos first
      if (servo2.attached()) servo2.detach();
      if (servo3.attached()) servo3.detach();
      if (servo4.attached()) servo4.detach();
      
      // Attach and move only right hand servo
      if (!servo1.attached()) servo1.attach(9);
      
      // Play audio with speaking animation
      if (!audioPlaying) {
        dfplayer.play(2);
        audioPlaying = true;
        lastAudioTime = millis();
        audioStartTime = millis();
        audioJustStarted = true;
        startEyeAnimation(1); // Start speaking animation
      }
      
      smoothMoveEase(servo1, currentRight, 180, 2000);
      rightPlayed = true;
      audioPlaying = false;
      // Speaking animation will stop automatically after delay
    } 
    else if (command == "lower your right hand") {
      // Detach other servos first
      if (servo2.attached()) servo2.detach();
      if (servo3.attached()) servo3.detach();
      if (servo4.attached()) servo4.detach();
      
      // Attach and move only right hand servo
      if (!servo1.attached()) servo1.attach(9);
      
      smoothMoveEase(servo1, currentRight, 0, 2000);
      rightPlayed = false;
      // No audio for lowering
    }
    
    // LEFT HAND COMMANDS
    else if (command == "raise your left hand" && !leftPlayed) {
      // Detach other servos first
      if (servo1.attached()) servo1.detach();
      if (servo3.attached()) servo3.detach();
      if (servo4.attached()) servo4.detach();
      
      // Attach and move only left hand servo
      if (!servo2.attached()) servo2.attach(10);
      
      // Play audio with speaking animation
      if (!audioPlaying) {
        dfplayer.play(1);
        audioPlaying = true;
        lastAudioTime = millis();
        audioStartTime = millis();
        audioJustStarted = true;
        startEyeAnimation(1); // Start speaking animation
      }
      
      smoothMoveEase(servo2, currentLeft, 0, 2000);
      leftPlayed = true;
      audioPlaying = false;
      // Speaking animation will stop automatically after delay
    } 
    else if (command == "lower your left hand") {
      // Detach other servos first
      if (servo1.attached()) servo1.detach();
      if (servo3.attached()) servo3.detach();
      if (servo4.attached()) servo4.detach();
      
      // Attach and move only left hand servo
      if (!servo2.attached()) servo2.attach(10);
      
      smoothMoveEase(servo2, currentLeft, 180, 2000);
      leftPlayed = false;
      // No audio for lowering
    }
    
    // HEAD COMMANDS
    else if (command == "look to the right" && !headPlayed) {
      // Detach other servos first
      if (servo1.attached()) servo1.detach();
      if (servo2.attached()) servo2.detach();
      if (servo4.attached()) servo4.detach();
      
      // Attach and prepare head movement
      if (!servo3.attached()) servo3.attach(11);
      
      // Play audio with speaking animation
      if (!audioPlaying) {
        dfplayer.play(7);
        audioPlaying = true;
        lastAudioTime = millis();
        audioStartTime = millis();
        audioJustStarted = true;
        startEyeAnimation(1); // Start speaking animation
      }
      
      headState = "LEFT";
      headPlayed = true;
      audioPlaying = false;
      // Speaking animation will stop automatically after delay
    } 
    else if (command == "look to the left" && !headPlayed) {
      // Detach other servos first
      if (servo1.attached()) servo1.detach();
      if (servo2.attached()) servo2.detach();
      if (servo4.attached()) servo4.detach();
      
      // Attach and prepare head movement
      if (!servo3.attached()) servo3.attach(11);
      
      // Play audio with speaking animation
      if (!audioPlaying) {
        dfplayer.play(6);
        audioPlaying = true;
        lastAudioTime = millis();
        audioStartTime = millis();
        audioJustStarted = true;
        startEyeAnimation(1); // Start speaking animation
      }
      
      headState = "RIGHT";
      headPlayed = true;
      audioPlaying = false;
      // Speaking animation will stop automatically after delay
    } 
    else if (command == "look straight ahead" || command == "none") {
      // Detach other servos first
      if (servo1.attached()) servo1.detach();
      if (servo2.attached()) servo2.detach();
      if (servo4.attached()) servo4.detach();
      
      // Attach and prepare head movement
      if (!servo3.attached()) servo3.attach(11);
      
      headState = "CENTER";
      headPlayed = false;
      // No audio for centering
    }
    
    // BODY COMMANDS
    else if (command == "turn to the right" && !bodyActive) {
      // Detach other servos first
      if (servo1.attached()) servo1.detach();
      if (servo2.attached()) servo2.detach();
      if (servo3.attached()) servo3.detach();
      
      // Attach and move body
      attachBodyServo();
      
      // Play audio with speaking animation
      if (!audioPlaying) {
        dfplayer.play(4);
        audioPlaying = true;
        lastAudioTime = millis();
        audioStartTime = millis();
        audioJustStarted = true;
        startEyeAnimation(1); // Start speaking animation
      }
      
      smoothMoveEase(servo4, currentBody, 0, 2500);
      bodyActive = true; // Mark body movement as active
      audioPlaying = false;
      // Speaking animation will stop automatically after delay
    } 
    else if (command == "turn to the left" && !bodyActive) {
      // Detach other servos first
      if (servo1.attached()) servo1.detach();
      if (servo2.attached()) servo2.detach();
      if (servo3.attached()) servo3.detach();
      
      // Attach and move body
      attachBodyServo();
      
      // Play audio with speaking animation
      if (!audioPlaying) {
        dfplayer.play(3);
        audioPlaying = true;
        lastAudioTime = millis();
        audioStartTime = millis();
        audioJustStarted = true;
        startEyeAnimation(1); // Start speaking animation
      }
      
      smoothMoveEase(servo4, currentBody, 180, 2500);
      bodyActive = true; // Mark body movement as active
      audioPlaying = false;
      // Speaking animation will stop automatically after delay
    } 
    else if (command == "stand straight" && !bodyActive) {
      // Detach other servos first
      if (servo1.attached()) servo1.detach();
      if (servo2.attached()) servo2.detach();
      if (servo3.attached()) servo3.detach();
      
      // Attach and move body
      attachBodyServo();
      
      smoothMoveEase(servo4, currentBody, 90, 2500);
      bodyActive = true; // Mark body movement as active
      // No audio for standing straight
    }
    else if (command == "introduce yourself") {
      // Play audio with speaking animation
      if (!audioPlaying) {
        dfplayer.play(5);
        audioPlaying = true;
        lastAudioTime = millis();
        audioStartTime = millis();
        audioJustStarted = true;
        startEyeAnimation(1); // Start speaking animation
        audioPlaying = false;
      }
    }
    
    // If body command is repeated while active, ignore it
    else if ((command == "turn to the right" || 
              command == "turn to the left" || 
              command == "stand straight") && bodyActive) {
      Serial.println("Body movement already active. Send different command.");
    }
    
    servoBusy = false; // Reset busy flag
  }
}

// Smooth human-like servo movement
void smoothMoveEase(Servo &servo, int &currentAngle, int targetAngle, int duration_ms) {
  int steps = abs(targetAngle - currentAngle);
  if (steps == 0) return;
  
  // Calculate step time
  int stepDelay = duration_ms / steps;
  
  for (int i = 0; i <= steps; i++) {
    float t = (float)i / steps;
    float ease = 0.5 - 0.5 * cos(t * PI);
    int angle = currentAngle + (int)(ease * (targetAngle - currentAngle));
    servo.write(angle);
    delay(stepDelay);
  }
  currentAngle = targetAngle;
  
  // Small delay after movement to settle
  delay(100);
}

// Attach body servo temporarily
void attachBodyServo() {
  if (!bodyAttached) {
    servo4.attach(6);
    bodyAttached = true;
  }
}

// Detach body servo
void detachBodyServo() {
  if (bodyAttached) {
    servo4.detach();
    bodyAttached = false;
    bodyActive = false; // Reset body activity flag
  }
}

// Control head movement smoothly
void handleHeadMovement() {
  // Only move head if servo3 is attached
  if (!servo3.attached()) return;
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= speedDelay) {
    previousMillis = currentMillis;
    
    if (headState == "RIGHT" && currentHead < 180) {
      currentHead++;
      servo3.write(currentHead);
    }
    else if (headState == "LEFT" && currentHead > 0) {
      currentHead--;
      servo3.write(currentHead);
    }
    else if (headState == "CENTER") {
      if (currentHead < 90) {
        currentHead++;
        servo3.write(currentHead);
      }
      else if (currentHead > 90) {
        currentHead--;
        servo3.write(currentHead);
      }
    }
  }
}

// Intro animation with 10° body sway
void handleIntroAnimation() {
  unsigned long elapsed = millis() - introStartTime;

  if (elapsed < 2000) {
    smoothMoveEase(servo4, currentBody, 100, 1000); // right
  } 
  else if (elapsed < 4000) {
    smoothMoveEase(servo4, currentBody, 80, 1000);  // left
  } 
  else if (elapsed < 5500) {
    smoothMoveEase(servo4, currentBody, 90, 1000);  // center
  } 
  else if (elapsed >= introDuration) {
    introducing = false;
    detachBodyServo();
    Serial.println("🎤 معرفی پایان یافت");
  }
}

// ====== EYE ANIMATION FUNCTIONS ======

// Start eye animation (0: breathing, 1: speaking)
void startEyeAnimation(int type) {
  eyeAnimationActive = true;
  eyeAnimationType = type;
  eyeAnimationStart = millis();
  eyeAnimationProgress = 0;
  Serial.print("Starting eye animation: ");
  Serial.println(type == 0 ? "Breathing" : "Speaking");
}

// Stop eye animation
void stopEyeAnimation() {
  eyeAnimationActive = false;
  eyeAnimationProgress = 0;
}

// Handle eye animations
void handleEyeAnimations() {
  if (!eyesOn) return; // No animation if eyes are off
  
  unsigned long currentMillis = millis();
  
  // Update eyes every 20ms for smooth animation
  if (currentMillis - lastEyeUpdate >= 20) {
    lastEyeUpdate = currentMillis;
    
    if (eyeAnimationActive) {
      if (eyeAnimationType == 0) {
        // IDLE BREATHING ANIMATION
        // Smooth breathing effect: 0% -> 100% -> 0% brightness
        unsigned long elapsed = currentMillis - eyeAnimationStart;
        eyeAnimationProgress = (elapsed % BREATHING_CYCLE) / (float)BREATHING_CYCLE;
        
        // Use sine wave for natural breathing: 0.5 + 0.5 * sin(2π * progress)
        float breathValue = 0.5 + 0.5 * sin(2 * PI * eyeAnimationProgress);
        
        // Map to brightness: 100 to 255 (dim to bright)
        int brightness = 100 + (int)(breathValue * 155);
        analogWrite(eyesPin, brightness);
        currentEyeBrightness = brightness;
      }
      else if (eyeAnimationType == 1) {
        // SPEAKING ANIMATION
        // Faster pulsing while speaking
        unsigned long elapsed = currentMillis - eyeAnimationStart;
        eyeAnimationProgress = (elapsed % SPEAKING_CYCLE) / (float)SPEAKING_CYCLE;
        
        // Faster sine wave for speaking effect
        float speakValue = 0.5 + 0.5 * sin(2 * PI * eyeAnimationProgress);
        
        // More dramatic pulsing: 150 to 255
        int brightness = 150 + (int)(speakValue * 105);
        analogWrite(eyesPin, brightness);
        currentEyeBrightness = brightness;
        
        // Auto-stop speaking animation after 3 seconds if no new audio
        if (currentMillis - audioStartTime > 3000) {
          // Return to idle breathing
          startEyeAnimation(0);
        }
      }
    }
    else {
      // If no animation active, start idle breathing
      startEyeAnimation(0);
    }
  }
  
  // Handle audio just started flag
  if (audioJustStarted && (currentMillis - audioStartTime > 100)) {
    audioJustStarted = false;
  }
}

// Set eyes to specific brightness immediately
void setEyes(int brightness) {
  brightness = constrain(brightness, 0, 255);
  analogWrite(eyesPin, brightness);
  currentEyeBrightness = brightness;
}

// ====== AUTO-SHUTDOWN FUNCTIONS ======

// Reset the activity timer (call this on any activity)
void resetActivityTimer() {
  lastActivityTime = millis();
  shutdownWarningPlayed = false;
  Serial.println("Activity detected - Auto-shutdown timer reset");
}

// Handle auto-shutdown logic
void handleAutoShutdown() {
  if (!eyesOn || !servosEnabled) return; // Only if robot is ON
  
  unsigned long currentMillis = millis();
  unsigned long idleTime = currentMillis - lastActivityTime;
  
  // Check for actual shutdown
  if (idleTime >= shutdownTimeout) {
    Serial.println("⏰ Auto-shutdown: No activity for 60 seconds");
    
    // Turn off robot
    eyesOn = false;
    fadeEyesOff();
    dfplayer.play(8); // Eyes OFF sound
    detachAllServos();
    servosEnabled = false;
    stopEyeAnimation();
    
    Serial.println("🤖 Robot auto-shutdown complete");
    
    // Reset shutdown timer
    lastActivityTime = currentMillis;
    shutdownWarningPlayed = false;
  }
  
  // Optional: Display remaining time in serial monitor
  static unsigned long lastDisplayTime = 0;
  if (currentMillis - lastDisplayTime >= 5000) { // Every 5 seconds
    lastDisplayTime = currentMillis;
    unsigned long remainingTime = (shutdownTimeout - idleTime) / 1000;
    if (remainingTime > 0 && remainingTime <= 60) {
      Serial.print("Auto-shutdown in: ");
      Serial.print(remainingTime);
      Serial.println(" seconds");
    }
  }
}