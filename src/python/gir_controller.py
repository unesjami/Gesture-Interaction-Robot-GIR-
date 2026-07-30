import cv2
import mediapipe as mp
import serial
import time

time.sleep(2)  # wait 2 seconds for Bluetooth connection

arduino = serial.Serial('COM5', 9600)  # پورت سریال خودت رو تنظیم کن
time.sleep(2)

mpHands = mp.solutions.hands
hands = mpHands.Hands(max_num_hands=2, model_complexity=1, min_detection_confidence=0.7)
mpDraw = mp.solutions.drawing_utils

cap = cv2.VideoCapture(0)
tipIds = [4, 8, 12, 16, 20]

rightActive = False
leftActive = False
introActive = False  # معرفی ربات

while True:
    success, img = cap.read()
    img = cv2.flip(img, 1)
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(imgRGB)

    display_text = ""  # متنی که بالای صفحه نمایش داده میشه

    if results.multi_hand_landmarks and results.multi_handedness:
        for i, hand_handedness in enumerate(results.multi_handedness):
            handType = hand_handedness.classification[0].label
            lmList = []
            handLms = results.multi_hand_landmarks[i]

            for id, lm in enumerate(handLms.landmark):
                h, w, _ = img.shape
                cx, cy = int(lm.x * w), int(lm.y * h)
                lmList.append((cx, cy))

            mpDraw.draw_landmarks(img, handLms, mpHands.HAND_CONNECTIONS)

            fingers = []
            if handType == "Right":
                fingers.append(1 if lmList[tipIds[0]][0] > lmList[tipIds[0] - 1][0] else 0)  # شست
            else:
                fingers.append(1 if lmList[tipIds[0]][0] < lmList[tipIds[0] - 1][0] else 0)

            for i in range(1, 5):
                fingers.append(1 if lmList[tipIds[i]][1] < lmList[tipIds[i] - 2][1] else 0)

            numFingersUp = sum(fingers)

            # ✅ معرفی ربات: وقتی 4 انگشت دست راست بالا باشد
            if handType == "Right" and numFingersUp == 3:
                if not introActive:  # فقط یک بار اجرا شود
                    arduino.write(b'introduce yourself\n')   # دستور پخش فایل صوتی به آردوینو
                    introActive = True
                    display_text += "-- Robot Intro -- "
            else:
                introActive = False

            # کنترل دست‌ها
            if numFingersUp == 4:
                if handType == "Right":
                    arduino.write(b"raise your right hand\n")
                    rightActive = True
                    display_text += "-- Right Hand -- "
                elif handType == "Left":
                    arduino.write(b'raise your left hand\n')
                    leftActive = True
                    display_text += "-- Left Hand -- "
            else:
                if handType == "Right" and rightActive:
                    arduino.write(b'lower your right hand\n')
                    rightActive = False
                elif handType == "Left" and leftActive:
                    arduino.write(b'lower your left hand\n')
                    leftActive = False

            # کنترل سر ربات (فقط انگشت اشاره)
            if handType == "Right" and numFingersUp == 2 and fingers[1] == 1:
                x_index = lmList[8][0]
                display_text += "-- Robot Head -- "
                if x_index < w / 3:
                    arduino.write(b'look to the left\n')
                elif x_index > 2 * w / 3:
                    arduino.write(b'look to the right\n')
                else:
                    arduino.write(b'look straight ahead\n')

            # ✅ کنترل بدن ربات (شست + اشاره)
            elif handType == "Right" and numFingersUp == 1 and fingers[1] == 1:
                x_index = lmList[8][0]
                display_text += "-- Robot Body -- "
                if x_index < w / 3:
                    arduino.write(b'turn to the left\n')
                elif x_index > 2 * w / 3:
                    arduino.write(b'turn to the right\n')
                else:
                    arduino.write(b'stand straight\n')
            elif numFingersUp != 1:
                arduino.write(b'NONE\n')

    else:
        if rightActive:
            arduino.write(b'lower your right hand\n')
            rightActive = False
        if leftActive:
            arduino.write(b'lower your left hand\n')
            leftActive = False
        arduino.write(b'NONE\n')
        arduino.write(b'stand straight\n')  # وقتی دست در تصویر نیست، سروو بدن به ۹۰ برگردد

    # نمایش متن در بالا وسط تصویر
    if display_text != " Gesture Interaction Robot ":
        text_size = cv2.getTextSize(display_text.strip(), cv2.FONT_HERSHEY_SIMPLEX, 1, 2)[0]
        text_x = (img.shape[1] - text_size[0]) // 2
        cv2.putText(img, display_text.strip(), (text_x, 50),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    cv2.imshow("Image", img)
    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
arduino.close()
