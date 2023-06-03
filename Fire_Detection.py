import cv2
import serial
import time

# Load the Haar Cascade classifier for fire detection
fire_cascade = cv2.CascadeClassifier('fire_detection.xml')

# Create a VideoCapture object to read from the webcam
cap = cv2.VideoCapture(0)

# Check if the webcam is opened correctly
if not cap.isOpened():
    raise IOError("Cannot open webcam")

# Set the resolution of the webcam to 1080p (1920x1080)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

# Initialize the serial port to communicate with the servo motor
ser = serial.Serial('COM9', 9600)

# Loop over frames from the webcam
while True:
    # Read a frame from the webcam
    ret, frame = cap.read()

    # Convert to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect fire using the Haar Cascade classifier
    fires = fire_cascade.detectMultiScale(gray, scaleFactor=1.05, minNeighbors=5, minSize=(30, 30))

    # Draw a bounding box around each detected fire and print its center coordinates
    for (x, y, w, h) in fires:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
        cx = int(x + w / 2)
        cy = int(y + h / 2)
        mapped_x = int((cx / frame.shape[1]) * 90)
        mapped_y = int((cy / frame.shape[0]) * 90)
        print("Fire detected at ({}, {})".format(mapped_x, mapped_y))
        print(str(mapped_x) + ',' + str(mapped_y) + '\n')

        # Calculate the angles to rotate the servo motor
        # angle_x = mapped_x - 90
        # angle_y = mapped_y - 90

        # Send the angles to the servo motor using the serial port

        ser.write(bytes(str(mapped_x) + ',' + str(mapped_y) + '\n', 'utf-8'))

    # Show the frame on the screen
    cv2.imshow("Fire Detection", frame)

    # Wait for a key press (1 millisecond)
    key = cv2.waitKey(1)

    # If the 'q' key is pressed, break from the loop
    if key == ord('q'):
        break
    time.sleep(2)

# Release the VideoCapture object, close all windows, and close the serial port
cap.release()
cv2.destroyAllWindows()
ser.close()
