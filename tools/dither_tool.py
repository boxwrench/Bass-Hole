#!/usr/bin/env python3
"""
dither_tool.py - Apply Floyd-Steinberg dithering with custom palette

Reduces images to a limited color palette using dithering for a retro aesthetic.
Designed for the Bass Hole 16-color NorCal palette.

Usage:
    python dither_tool.py input.png --palette palette.hex --output dithered.png
    python dither_tool.py input.png --colors 16  # Auto-generate palette

Requirements:
    pip install Pillow numpy

"""

import argparse
import sys
from pathlib import Path

try:
    from PIL import Image
    import numpy as np
except ImportError:
    print("Error: Required libraries missing. Install with:")
    print("  pip install Pillow numpy")
    sys.exit(1)


# Default 16-color NorCal palette
DEFAULT_PALETTE = [
    (0x00, 0x1F, 0x3F),  # Deep Blue
    (0x00, 0x74, 0xD9),  # Ocean Blue
    (0x7F, 0xDB, 0xFF),  # Aqua
    (0x39, 0xCC, 0xCC),  # Teal
    (0x3D, 0x99, 0x70),  # Olive
    (0x2E, 0xCC, 0x40),  # Green
    (0x01, 0xFF, 0x70),  # Lime
    (0x8B, 0x45, 0x13),  # Brown
    (0xD2, 0xB4, 0x8C),  # Tan
    (0xAA, 0xAA, 0xAA),  # Gray
    (0xDD, 0xDD, 0xDD),  # Silver
    (0xFF, 0xFF, 0xFF),  # White
    (0x00, 0x00, 0x00),  # Black
    (0x33, 0x33, 0x33),  # Dark Gray
    (0xFF, 0x85, 0x1B),  # Orange
    (0xFF, 0x41, 0x36),  # Red
]


def hex_to_rgb(hex_str):
    """Convert hex color string to RGB tuple."""
    hex_str = hex_str.lstrip('#')
    return tuple(int(hex_str[i:i+2], 16) for i in (0, 2, 4))


def load_palette(palette_path):
    """Load palette from hex file (one color per line)."""
    colors = []
    with open(palette_path, 'r') as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith(('/', '#', ';')):
                # Handle both "#RRGGBB" and "RRGGBB" formats
                if line.startswith('#'):
                    line = line[1:]
                if len(line) >= 6:
                    try:
                        colors.append(hex_to_rgb(line[:6]))
                    except ValueError:
                        pass
    return colors if colors else DEFAULT_PALETTE


def find_closest_color(pixel, palette):
    """Find the closest palette color to the given pixel using Euclidean distance."""
    min_dist = float('inf')
    closest = palette[0]

    for color in palette:
        # Weighted RGB distance (human eye is more sensitive to green)
        dr = pixel[0] - color[0]
        dg = pixel[1] - color[1]
        db = pixel[2] - color[2]
        dist = dr*dr*0.299 + dg*dg*0.587 + db*db*0.114

        if dist < min_dist:
            min_dist = dist
            closest = color

    return closest


def floyd_steinberg_dither(img, palette):
    """
    Apply Floyd-Steinberg dithering to an image.

    Distributes quantization error to neighboring pixels for smoother appearance.
    """
    # Convert to numpy array for faster processing
    pixels = np.array(img, dtype=np.float32)
    height, width = pixels.shape[:2]

    for y in range(height):
        for x in range(width):
            old_pixel = pixels[y, x].copy()
            new_pixel = find_closest_color(tuple(old_pixel.astype(int)), palette)
            pixels[y, x] = new_pixel

            # Calculate error
            error = old_pixel - np.array(new_pixel, dtype=np.float32)

            # Distribute error to neighbors (Floyd-Steinberg pattern)
            # Right: 7/16, Bottom-left: 3/16, Bottom: 5/16, Bottom-right: 1/16
            if x + 1 < width:
                pixels[y, x + 1] += error * 7 / 16
            if y + 1 < height:
                if x > 0:
                    pixels[y + 1, x - 1] += error * 3 / 16
                pixels[y + 1, x] += error * 5 / 16
                if x + 1 < width:
                    pixels[y + 1, x + 1] += error * 1 / 16

        # Progress indicator for large images
        if y % 50 == 0:
            print(f"  Processing: {y}/{height} rows...", end='\r', file=sys.stderr)

    print(f"  Processing: {height}/{height} rows... done", file=sys.stderr)

    # Clip values and convert back to uint8
    pixels = np.clip(pixels, 0, 255).astype(np.uint8)
    return Image.fromarray(pixels)


def ordered_dither(img, palette, matrix_size=4):
    """
    Apply ordered (Bayer) dithering for a more uniform pattern.

    Alternative to Floyd-Steinberg, produces visible pattern but no "noise".
    """
    # Bayer matrix (4x4)
    bayer_4x4 = np.array([
        [0,  8,  2,  10],
        [12, 4,  14, 6],
        [3,  11, 1,  9],
        [15, 7,  13, 5]
    ]) / 16.0 - 0.5

    pixels = np.array(img, dtype=np.float32)
    height, width = pixels.shape[:2]

    # Apply threshold matrix
    for y in range(height):
        for x in range(width):
            threshold = bayer_4x4[y % 4, x % 4] * 64  # Scale threshold
            adjusted = pixels[y, x] + threshold
            adjusted = np.clip(adjusted, 0, 255)
            new_pixel = find_closest_color(tuple(adjusted.astype(int)), palette)
            pixels[y, x] = new_pixel

    return Image.fromarray(pixels.astype(np.uint8))


def main():
    parser = argparse.ArgumentParser(
        description='Apply dithering with custom color palette',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    python dither_tool.py fish.png --output fish_dithered.png
    python dither_tool.py fish.png --palette my_palette.hex
    python dither_tool.py fish.png --method ordered --output fish_bayer.png

Palette file format (one hex color per line):
    #001F3F
    #0074D9
    #7FDBFF
    ...

Default palette is the 16-color NorCal theme.
        """
    )
    parser.add_argument('input', help='Input image file')
    parser.add_argument('--palette', '-p', help='Palette file (hex colors, one per line)')
    parser.add_argument('--output', '-o', help='Output file (default: input_dithered.png)')
    parser.add_argument('--method', '-m', choices=['floyd', 'ordered'],
                        default='floyd', help='Dithering method (default: floyd)')
    parser.add_argument('--colors', '-c', type=int,
                        help='Auto-generate palette with N colors (ignores --palette)')
    parser.add_argument('--preview', action='store_true',
                        help='Show preview window (requires display)')

    args = parser.parse_args()

    # Check input
    input_path = Path(args.input)
    if not input_path.exists():
        print(f"Error: Input file not found: {args.input}", file=sys.stderr)
        sys.exit(1)

    # Load image
    print(f"Loading: {args.input}", file=sys.stderr)
    img = Image.open(args.input)
    if img.mode != 'RGB':
        img = img.convert('RGB')

    print(f"  Size: {img.width}x{img.height}", file=sys.stderr)

    # Determine palette
    if args.colors:
        # Auto-generate palette from image
        print(f"  Generating {args.colors}-color palette...", file=sys.stderr)
        quantized = img.quantize(colors=args.colors, method=Image.MEDIANCUT)
        palette_data = quantized.getpalette()[:args.colors * 3]
        palette = [tuple(palette_data[i:i+3]) for i in range(0, len(palette_data), 3)]
    elif args.palette:
        print(f"  Loading palette: {args.palette}", file=sys.stderr)
        palette = load_palette(args.palette)
    else:
        print(f"  Using default 16-color NorCal palette", file=sys.stderr)
        palette = DEFAULT_PALETTE

    print(f"  Palette: {len(palette)} colors", file=sys.stderr)

    # Apply dithering
    print(f"  Applying {args.method} dithering...", file=sys.stderr)
    if args.method == 'ordered':
        result = ordered_dither(img, palette)
    else:
        result = floyd_steinberg_dither(img, palette)

    # Output
    if args.output:
        output_path = args.output
    else:
        output_path = input_path.stem + '_dithered' + input_path.suffix

    result.save(output_path)
    print(f"Saved: {output_path}", file=sys.stderr)

    # Preview
    if args.preview:
        try:
            result.show()
        except Exception:
            print("  Preview not available", file=sys.stderr)


if __name__ == '__main__':
    main()
