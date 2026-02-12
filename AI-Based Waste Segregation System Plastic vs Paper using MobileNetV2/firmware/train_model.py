import torch
import torch.nn as nn
import torch.optim as optim
from torchvision import datasets, transforms, models
from torch.utils.data import DataLoader

# --------------------
# CONFIG
# --------------------
DATA_DIR = "dataset"
MODEL_PATH = "plastic_paper_model.pth"
BATCH_SIZE = 16
NUM_EPOCHS = 10
LR = 0.0003
IMAGE_SIZE = 224
# --------------------

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print("Using:", device)

# --------------------
# TRANSFORMS
# --------------------
train_transforms = transforms.Compose([
    transforms.Resize((IMAGE_SIZE, IMAGE_SIZE)),
    transforms.RandomHorizontalFlip(),
    transforms.RandomRotation(10),
    transforms.ToTensor(),
    transforms.Normalize(
        [0.485, 0.456, 0.406],
        [0.229, 0.224, 0.225]
    )
])

# --------------------
# DATASET
# --------------------
train_data = datasets.ImageFolder(DATA_DIR, transform=train_transforms)
train_loader = DataLoader(train_data, batch_size=BATCH_SIZE, shuffle=True)

classes = train_data.classes
print("Classes found:", classes)

# --------------------
# MODEL
# --------------------
model = models.mobilenet_v2(weights=models.MobileNet_V2_Weights.IMAGENET1K_V1)

for param in model.features.parameters():
    param.requires_grad = False

model.classifier[1] = nn.Linear(model.last_channel, len(classes))
model = model.to(device)

# --------------------
# TRAIN SETUP
# --------------------
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.classifier.parameters(), lr=LR)

# --------------------
# TRAIN LOOP
# --------------------
for epoch in range(NUM_EPOCHS):
    model.train()
    running_loss = 0

    for images, labels in train_loader:
        images, labels = images.to(device), labels.to(device)

        optimizer.zero_grad()
        outputs = model(images)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        running_loss += loss.item()

    print(f"Epoch {epoch+1}/{NUM_EPOCHS}, Loss: {running_loss:.4f}")

# --------------------
# SAVE MODEL
# --------------------
torch.save(model.state_dict(), MODEL_PATH)
print("Model saved as:", MODEL_PATH)
