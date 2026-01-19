from PIL import Image
import os

dirs = ["assets/resized/fish", "assets/resized/ui"]

print("Sprite Dimensions:")
for d in dirs:
    if os.path.exists(d):
        for f in os.listdir(d):
            if f.endswith(".png"):
                img = Image.open(os.path.join(d, f))
                print(f"{f}: {img.width}x{img.height}")
