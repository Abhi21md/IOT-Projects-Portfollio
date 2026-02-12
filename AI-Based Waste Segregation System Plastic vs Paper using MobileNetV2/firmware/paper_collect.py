import cv2
import os
import time

# ---------- CONFIG ----------
CAMERA_INDEX = 0
SAVE_DIR = "dataset/paper"
MAX_IMAGES = 500
FRAME_W, FRAME_H = 640, 480
SAVE_DELAY = 0.05   # seconds (smaller = faster)
# ----------------------------

os.makedirs(SAVE_DIR, exist_ok=True)

cap = cv2.VideoCapture(CAMERA_INDEX)
if not cap.isOpened():
    print("Could not open USB camera")
    exit()

count = len(os.listdir(SAVE_DIR))
print("Auto PAPER collection started...")

while count < MAX_IMAGES:
    ret, frame = cap.read()
    if not ret:
        continue

    frame = cv2.resize(frame, (FRAME_W, FRAME_H))

    filename = f"{count:05d}.jpg"
    cv2.imwrite(os.path.join(SAVE_DIR, filename), frame)
    count += 1

    cv2.putText(frame, f"PAPER COLLECTING: {count}/{MAX_IMAGES}",
                (10, 30), cv2.FONT_HERSHEY_SIMPLEX,
                0.8, (0, 255, 0), 2)

    cv2.imshow("Paper Auto Collector", frame)
    cv2.waitKey(1)

    time.sleep(SAVE_DELAY)

print("Paper collection completed.")
cap.release()
cv2.destroyAllWindows()
