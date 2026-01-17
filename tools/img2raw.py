#!/usr/bin/env python3
"""
img2raw.py - Convert images to raw RGB565 binary files for SD card storage

Converts PNG/JPG images to raw RGB565 binary format that can be loaded
directly from SD card and rendered with TFT_eSPI pushImage().

Usage:
    python img2raw.py input.png output.bin
    python img2raw.py input.png  # Outputs to input.bin

Requirements:
    pip install Pillow

Output format:
    Raw binary RGB565 (16-bit color: 5 bits red, 6 bits green, 5 bits blue)
    Little-endian byte order (LSB first)
    File size = width × height × 2 bytes
"""

import argparse
import sys
import struct
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    print("Error: Pillow library required. Install with: pip install Pillow")
    sys.exit(1)


def rgb_to_rgb565(r, g, b):
    """Convert 8-bit RGB to 16-bit RGB565."""
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def convert_image_to_binary(input_path, output_path, transparent_color=None):
    """
    Convert image to raw RGB565 binary file.

    Args:
        input_path: Path to input image
        output_path: Path to output binary file
        transparent_color: RGB tuple for transparency (uses magenta 0xF81F)

    Returns:
        Tuple of (width, height, file_size)
    """
    img = Image.open(input_path)

    # Convert to RGBA to handle transparency
    if img.mode != 'RGBA':
        img = img.convert('RGBA')

    width, height = img.size
    pixels = list(img.getdata())

    # Open output file in binary write mode
    with open(output_path, 'wb') as f:
        for pixel in pixels:
            r, g, b, a = pixel

            # Handle transparency (use magenta as transparent color)
            if a < 128:
                rgb565 = 0xF81F  # Magenta
            else:
                rgb565 = rgb_to_rgb565(r, g, b)

            # Write as little-endian 16-bit value
            f.write(struct.pack('<H', rgb565))

    file_size = width * height * 2
    return width, height, file_size


def main():
    parser = argparse.ArgumentParser(
        description='Convert images to raw RGB565 binary files for SD card',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    python img2raw.py fish.png fish.bin
    python img2raw.py background.png  # Creates background.bin

The output is raw RGB565 binary format compatible with TFT_eSPI pushImage().
Transparent pixels (alpha < 128) are converted to magenta (0xF81F).

File size = width × height × 2 bytes
Example: 64×64 sprite = 8,192 bytes
        """
    )
    parser.add_argument('input', help='Input image file (PNG, JPG, etc.)')
    parser.add_argument('output', nargs='?', help='Output binary file (default: input.bin)')
    parser.add_argument('--quiet', '-q', action='store_true', help='Suppress info messages')

    args = parser.parse_args()

    # Check input exists
    input_path = Path(args.input)
    if not input_path.exists():
        print(f"Error: Input file not found: {args.input}", file=sys.stderr)
        sys.exit(1)

    # Determine output path
    if args.output:
        output_path = Path(args.output)
    else:
        output_path = input_path.with_suffix('.bin')

    # Convert
    try:
        width, height, file_size = convert_image_to_binary(input_path, output_path)
    except Exception as e:
        print(f"Error converting image: {e}", file=sys.stderr)
        sys.exit(1)

    # Report
    if not args.quiet:
        print(f"Converted {input_path.name} ({width}×{height}) → {output_path.name}")
        print(f"File size: {file_size:,} bytes ({file_size / 1024:.2f} KB)")


if __name__ == '__main__':
    main()
