#THE ORGINAL CODE CAME FROM: https://github.com/googlesamples/mediapipe/tree/main AND HAVE BEEN MODIFYEDE
import cv2
import mediapipe as mp
import os
import serial
import numpy as np

###*[PROGRAM START]###
print("\n\n\n\n\n\n\n\n#######################PROGRAM STARTS#######################")

###*[VARIABLES]###
print("Variables section started")
serialportName = 'COM3'
cap = cv2.VideoCapture(0)
mpHands = mp.solutions.hands
hands = mpHands.Hands()
mpDraw = mp.solutions.drawing_utils
folder = os.path.join(os.getcwd(), "HeighMap")
heightmaps = os.listdir(folder)
heighMap = cv2.imread(os.path.join(folder, heightmaps[0]), cv2.IMREAD_GRAYSCALE)
hmPreview = None
signal = 0
prevSignal = 0
arduinoSerial = serial.Serial(port=serialportName, baudrate=9600, timeout=.1)
isReadyToSend = True
print("Variables Finished")

###*[SETUP]###
print("Setup Started")
arduinoSerial.write(bytes(str(100), 'utf-8'))  # write a string
print(folder)
print("Setup done")

def write_read_serial(x, rts):
    if rts and x > 0:
        print("The input signal" + str(x))
        arduinoSerial.write(bytes(str(x), 'utf-8'))
        rts = False
    else:
        inputArd = arduinoSerial.readline().decode('utf-8').strip()
        print(inputArd)
        if inputArd == "r":
            rts = True
    return rts

###*[LOOP]###
print("Loop started")
while True:
    success, image = cap.read()
    image = cv2.flip(image, 1)
    hmPreview = heighMap.copy()
    imageRGB = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = hands.process(imageRGB)

    # checking whether a hand is detected
    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks:  # working with each hand
            for id, lm in enumerate(handLms.landmark):
                h, w, c = image.shape
                cx, cy = int(lm.x * w), int(lm.y * h)
                if id == 8:
                    try:
                        heightValue = int(heighMap[cx, cy])
                        cv2.circle(image, (cx, cy), 25, (255, 0, 255), cv2.FILLED)
                        cv2.circle(hmPreview, (cx, cy), 25, (heightValue, heightValue, heightValue), cv2.FILLED)
                        print("cx: " + str(cx) + " cy: " + str(cy))
                        hhm, whm = heighMap.shape
                        print("hhm: " + str(hhm) + " whm: " + str(whm))
                        hi, wi = image.shape[:2]
                        print("wi: " + str(wi) + " hi: " + str(hi))
                        finalX = int((cx / hi) * hhm)
                        finalY = int((cy / wi) * whm)
                        print("finalX: " + str(finalX) + " finalY: " + str(finalY))
                        prevSignal = signal
                        signal = heighMap[finalX, finalY]
                        print("signal" + str(signal) + " The pump: " + str(abs(prevSignal - signal)))

                    except:
                        print("An exception occurred")

            mpDraw.draw_landmarks(image, handLms, mpHands.HAND_CONNECTIONS)
    
    cv2.imshow("Input", image)
    cv2.imshow("Output", hmPreview)
    isReadyToSend = write_read_serial(abs(prevSignal - signal), isReadyToSend)

    if cv2.waitKey(1) & 0xFF == ord('q'):  # Check for 'q' key press to exit
        break

# Release the camera and close all OpenCV windows
cap.release()
cv2.destroyAllWindows()
