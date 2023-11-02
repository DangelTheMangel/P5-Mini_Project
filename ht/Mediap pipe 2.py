import cv2
import mediapipe as mp

cap = cv2.VideoCapture(0)
mpHands = mp.solutions.hands
hands = mpHands.Hands()
mpDraw = mp.solutions.drawing_utils

while True:
    success, image = cap.read()
    imageRGB = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = hands.process(imageRGB)

        # checking whether a hand is detected
    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks: # working with each hand
            for id, lm in enumerate(handLms.landmark):
                h, w, c = image.shape
                cx, cy = int(lm.x * w), int(lm.y * h)
                if id == 8 :
                    cv2.circle(image, (cx, cy), 25, (255, 0, 255), cv2.FILLED)

                    for (x, y, w, h) in id:
                        x1 = x
                        x2 = x + w
                        y1 = y
                        y2 = y + h
                    print ("diaginal point 1 (x1,y1) = ({},{})".format(x1, y1))
                    print ("diaginal point 2 (x2,y2) = ({},{})".format(x2, y2))

            mpDraw.draw_landmarks(image, handLms, mpHands.HAND_CONNECTIONS)
    


    cv2.imshow("Output", cv2.flip(image, 1))
    cv2.waitKey(1)
