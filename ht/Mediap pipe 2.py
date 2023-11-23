import cv2
import mediapipe as mp
import os
import serial
import matplotlib.pyplot as plt
import numpy as np

##*Variables*##
print("Variables section started")
serialportName = 'COM3'
cap = cv2.VideoCapture(0)
mpHands = mp.solutions.hands
hands = mpHands.Hands()
mpDraw = mp.solutions.drawing_utils
folder = os.path.join(os.getcwd(),"HeighMap")
heightmaps = os.listdir(folder)
heighMap = cv2.imread(os.path.join(folder,heightmaps[0]), cv2.IMREAD_GRAYSCALE)
hmPreview = None
resieHatMap = False
signal = np.zeros(5)
print("Variables Finnished")
##**SETUP**##
print("Setup Started")
#ser = serial.Serial(serialportName)  # open serial port
#print(ser.name)         # check which port was really used
#ser.write(b'b')     # write a string
#ser.close()
print(folder)
print("Setup done")

##**LOOP**##
print("Loop started")
while True:
    success, image = cap.read()
    image = cv2.flip(image, 1)
    if resieHatMap == False and success == True:
        heighMap = cv2.resize(heighMap, (image.shape[1], image.shape[0]))
        resieHatMap = True
    hmPreview = heighMap.copy()
    imageRGB = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = hands.process(imageRGB)
    print("Have image been captured succesfully: " + str(success) + " hands found: " + str(results))
    #lastPos = [0,0]
        # checking whether a hand is detected
    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks: # working with each hand
            for id, lm in enumerate(handLms.landmark):
                h, w, c = image.shape
                cx, cy = int(lm.x * w), int(lm.y * h)
                if id == 8 :
                    #lastPos = [cx,cy]
                    try:
                        heightValue = int(heighMap[cx,cy])
                        cv2.circle(image, (cx, cy), 25, (255, 0, 255), cv2.FILLED)
                        cv2.circle(hmPreview, (cx, cy), 25, (heightValue, heightValue, heightValue), cv2.FILLED)
                        print("cx: " + str(cx)+ " cy: " + str(cy) + " color value: " + str(heighMap[cx,cy]))
                        signal = np.insert(signal[1:], signal.size-1, heightValue)
                        #ser.write(str(heighMap[cx,cy]))     # write a string
                        #ser.close()  
                    except:
                        print("An exception occurred")
                    #print ("diaginal point 1 (x1,y1) = ({},{})".format(x1, y1))
                    #print ("diaginal point 2 (x2,y2) = ({},{})".format(x2, y2))

            mpDraw.draw_landmarks(image, handLms, mpHands.HAND_CONNECTIONS)
    cv2.imshow("Input", image)
    cv2.imshow("Output", hmPreview)
    plt.title("Line graph") 
    plt.xlabel("X axis") 
    plt.ylabel("Y axis") 
    plt.plot(signal, color ="red") 
    plt.show()
    cv2.waitKey(1)
    print("Images shown")
