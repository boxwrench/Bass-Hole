#!/usr/bin/env python3
"""
resize_sprites.py - Batch resize sprites for ESP32 CYD display

Resizes 256px source sprites to game-appropriate sizes while preserving
transparency. Outputs to assets/resized/ for conversion pipeline.

Usage:
    python resize_sprites.py                    # Resize all with defaults
    python resize_sprites.py --fish-size 48    # Custom fish size
    python resize_sprites.py --dry-run         # Preview without writing

Requirements:
    pip install Pillow
"""

import argparse
import sys
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    print("Error: Pillow library required. Install with: pip install Pillow")
    sys.exit(1)


# Default target sizes (width in pixels, height auto-calculated to maintain aspect)
DEFAULT_SIZES = {
    'fish': 32,        # Fish sprites
    'ty_knotts': 48,   # Ty Knotts character
    'bosses': 64,      # Boss characters
    'enemies': 40,     # Clankers, Cogsuckers
    'ui': 16,          # Coins, pellets, hearts
    'effects': 16,     # Bubbles, sparkles
    'backgrounds': 240 # Full game area backgrounds
}


def resize_image(input_path, output_path, target_size, maintain_aspect=True, auto_crop=True):
    """
    Resize image to target size while preserving transparency.

    Args:
        input_path: Source image path
        output_path: Destination path
        target_size: Target width (or tuple for exact size)
        maintain_aspect: If True, calculate height from aspect ratio
        auto_crop: If True, trim transparent padding before resize

    Returns:
        Tuple of (new_width, new_height) or None on error
    """
    try:
        img = Image.open(input_path)

        # Ensure RGBA for transparency
        if img.mode != 'RGBA':
            img = img.convert('RGBA')

        # Auto-crop: trim transparent padding
        if auto_crop:
            # Get bounding box of non-transparent pixels
            bbox = img.getbbox()
            if bbox:
                img = img.crop(bbox)

        orig_width, orig_height = img.size

        if isinstance(target_size, tuple):
            new_size = target_size
        elif maintain_aspect:
            # Calculate height maintaining aspect ratio
            aspect = orig_height / orig_width
            new_width = target_size
            new_height = int(target_size * aspect)
            new_size = (new_width, new_height)
        else:
            new_size = (target_size, target_size)

        # Use LANCZOS for high-quality downscaling
        resized = img.resize(new_size, Image.LANCZOS)

        # Ensure output directory exists
        output_path.parent.mkdir(parents=True, exist_ok=True)

        # Save as PNG to preserve transparency
        resized.save(output_path, 'PNG')

        return new_size

    except Exception as e:
        print(f"  Error resizing {input_path}: {e}", file=sys.stderr)
        return None


def get_category(filepath):
    """Determine sprite category from file path."""
    path_str = str(filepath).lower()

    if 'fish' in path_str:
        return 'fish'
    elif 'ty_knotts' in path_str:
        return 'ty_knotts'
    elif 'boss' in path_str:
        return 'bosses'
    elif 'enemy' in path_str or 'clanker' in path_str or 'cogsucker' in path_str:
        return 'enemies'
    elif 'ui' in path_str or 'coin' in path_str or 'heart' in path_str or 'button' in path_str or 'pellet' in path_str:
        return 'ui'
    elif 'effect' in path_str or 'bubble' in path_str or 'spark' in path_str or 'splash' in path_str:
        return 'effects'
    elif 'bg_' in path_str or 'background' in path_str:
        return 'backgrounds'
    else:
        return 'ui'  # Default to UI size


def process_sprites(source_dir, output_dir, sizes, dry_run=False, transparent_only=True):
    """
    Process all sprites in source directory.

    Args:
        source_dir: Path to assets/sprites/
        output_dir: Path to assets/resized/
        sizes: Dict of category -> target size
        dry_run: If True, only print what would be done
        transparent_only: If True, only process *_transparent.png files
    """
    source_path = Path(source_dir)
    output_path = Path(output_dir)

    if not source_path.exists():
        print(f"Error: Source directory not found: {source_dir}", file=sys.stderr)
        sys.exit(1)

    # Find all PNG files
    if transparent_only:
        pattern = '**/*_transparent.png'
    else:
        pattern = '**/*.png'

    png_files = list(source_path.glob(pattern))

    if not png_files:
        print(f"No PNG files found in {source_dir}", file=sys.stderr)
        sys.exit(1)

    print(f"Found {len(png_files)} sprites to process")
    print(f"Output directory: {output_path}")
    print()

    # Group by category for summary
    by_category = {}

    for png_file in sorted(png_files):
        category = get_category(png_file)
        target_size = sizes.get(category, 32)

        # Build output path (flatten structure, simplify names)
        # fish_r_trout_side_256_transparent.png -> fish_r_trout.png
        stem = png_file.stem
        stem = stem.replace('_256', '').replace('_transparent', '')
        stem = stem.replace('_side', '').replace('_idle', '_i').replace('_attack', '_a')
        stem = stem.replace('_fishing', '_f')

        out_file = output_path / category / f"{stem}.png"

        if category not in by_category:
            by_category[category] = []

        if dry_run:
            print(f"  [{category:12}] {png_file.name:45} -> {out_file.name} ({target_size}px)")
            by_category[category].append((png_file, out_file, target_size, None))
        else:
            result = resize_image(png_file, out_file, target_size)
            if result:
                print(f"  [{category:12}] {stem}.png ({result[0]}x{result[1]})")
                by_category[category].append((png_file, out_file, target_size, result))

    # Summary
    print()
    print("=" * 60)
    print("Summary by category:")
    for cat, items in sorted(by_category.items()):
        print(f"  {cat}: {len(items)} sprites at {sizes.get(cat, 32)}px")

    if dry_run:
        print()
        print("DRY RUN - no files written. Remove --dry-run to process.")
    else:
        print()
        print(f"Done! Resized sprites in: {output_path}")
        print("Next step: python img2code.py to convert to C arrays")


def main():
    parser = argparse.ArgumentParser(
        description='Batch resize sprites for ESP32 CYD',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    python resize_sprites.py                     # Resize all with defaults
    python resize_sprites.py --fish-size 48     # Larger fish
    python resize_sprites.py --dry-run          # Preview only
    python resize_sprites.py --all-pngs         # Include non-transparent

Default sizes:
    fish: 32px, ty_knotts: 48px, bosses: 64px
    enemies: 40px, ui: 16px, effects: 16px, backgrounds: 240px
        """
    )

    parser.add_argument('--source', '-s', default='assets/sprites',
                        help='Source sprites directory (default: assets/sprites)')
    parser.add_argument('--output', '-o', default='assets/resized',
                        help='Output directory (default: assets/resized)')
    parser.add_argument('--dry-run', '-n', action='store_true',
                        help='Preview without writing files')
    parser.add_argument('--all-pngs', action='store_true',
                        help='Process all PNGs, not just *_transparent.png')

    # Size overrides
    parser.add_argument('--fish-size', type=int, help='Fish sprite size')
    parser.add_argument('--char-size', type=int, help='Character (Ty) sprite size')
    parser.add_argument('--boss-size', type=int, help='Boss sprite size')
    parser.add_argument('--enemy-size', type=int, help='Enemy sprite size')
    parser.add_argument('--ui-size', type=int, help='UI element size')
    parser.add_argument('--effect-size', type=int, help='Effect sprite size')
    parser.add_argument('--bg-size', type=int, help='Background size')

    args = parser.parse_args()

    # Build sizes dict with overrides
    sizes = DEFAULT_SIZES.copy()
    if args.fish_size:
        sizes['fish'] = args.fish_size
    if args.char_size:
        sizes['ty_knotts'] = args.char_size
    if args.boss_size:
        sizes['bosses'] = args.boss_size
    if args.enemy_size:
        sizes['enemies'] = args.enemy_size
    if args.ui_size:
        sizes['ui'] = args.ui_size
    if args.effect_size:
        sizes['effects'] = args.effect_size
    if args.bg_size:
        sizes['backgrounds'] = args.bg_size

    process_sprites(
        args.source,
        args.output,
        sizes,
        dry_run=args.dry_run,
        transparent_only=not args.all_pngs
    )


if __name__ == '__main__':
    main()
