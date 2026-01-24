from PIL import Image
import os

def extract_tile(input_path, output_path, tile_x, tile_y, tile_size=32):
    img = Image.open(input_path)
    # tile_x and tile_y are indices (0, 1, 2...)
    left = tile_x * tile_size
    top = tile_y * tile_size
    right = left + tile_size
    bottom = top + tile_size
    
    tile = img.crop((left, top, right, bottom))
    # Resize to 64x64 if needed, but 32x32 is fine for tiling
    tile.save(output_path)
    print(f"Extracted tile from {tile_x},{tile_y} to {output_path}")

if __name__ == "__main__":
    if not os.path.exists("assets/backgrounds"):
        os.makedirs("assets/backgrounds")
    
    # Using Column 1, Row 4 as identified
    extract_tile("assets/backgrounds/tileset.png", "assets/backgrounds/water_tile.png", 1, 4, 32)
