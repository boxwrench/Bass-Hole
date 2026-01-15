from PIL import Image

# Create a 50x50 image to avoid scaling artifacts
width = 50
height = 50
img = Image.new('RGB', (width, height))

# Colors: Red, Green, Blue, White, Black
colors = [
    (255, 0, 0),    # Red
    (0, 255, 0),    # Green
    (0, 0, 255),    # Blue
    (255, 255, 255),# White
    (0, 0, 0)       # Black
]

# Draw 10px wide columns
pixels = []
for y in range(height):
    for x in range(width):
        col_idx = x // 10  # 0..4
        if col_idx < 5:
            pixels.append(colors[col_idx])
        else:
            pixels.append((0,0,0)) # Should not happen

img.putdata(pixels)
img.save('assets/test_colors.png')
print("Created assets/test_colors.png (50x50)")
