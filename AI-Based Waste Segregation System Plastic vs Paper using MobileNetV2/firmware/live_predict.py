import torch
import torch.nn as nn
from torchvision import transforms, models
import cv2
from PIL import Image

# --------------------
# CONFIG
# --------------------
CAMERA_INDEX = 0          # USB webcam
MODEL_PATH = "plastic_paper_model.pth"
IMAGE_SIZE = 224
CLASSES = ["paper", "plastic"]
# --------------------

# Load model
model = models.mobilenet_v2(weights=None)
model.classifier[1] = nn.Linear(model.last_channel, 2)

model.load_state_dict(torch.load(MODEL_PATH, map_location="cpu"))
model.eval()

print("Model loaded successfully")

# Transform
transform = transforms.Compose([
    transforms.Resize((IMAGE_SIZE, IMAGE_SIZE)),
    transforms.ToTensor(),
    transforms.Normalize(
        [0.485, 0.456, 0.406],
        [0.229, 0.224, 0.225]
    )
])

# Camera
cap = cv2.VideoCapture(CAMERA_INDEX)
if not cap.isOpened():
    print("Could not open USB camera")
    exit()

print("Live prediction started. Press Q to quit.")

while True:
    ret, frame = cap.read()
    if not ret:
        continue

    img_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    pil_img = Image.fromarray(img_rgb)

    input_tensor = transform(pil_img).unsqueeze(0)

    with torch.no_grad():
        outputs = model(input_tensor)
        probs = torch.softmax(outputs, dim=1)[0]
        conf, pred = torch.max(probs, 0)

    label = CLASSES[pred.item()]
    confidence = conf.item() * 100

    color = (0, 255, 0) if label == "plastic" else (255, 0, 0)
    cv2.putText(frame,
                f"{label.upper()} {confidence:.1f}%",
                (20, 40),
                cv2.FONT_HERSHEY_SIMPLEX,
                1,
                color,
                2)

    cv2.imshow("Plastic / Paper Live Prediction", frame)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()
