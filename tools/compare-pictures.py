#!/usr/bin/env python3
"""Compares the pictures two render backends drew of the same views.

Two backends do not give the same bytes (antialiasing, the rasteriser of glyphs), so the pictures
are compared by two numbers per pair (Documents/TechDocs/2026-11-19-r6-skia-backend-design.md, 4):

  different   the share of pixels that differ by more than the threshold in any channel
  misplaced   the same, but a pixel is forgiven when the other picture has a close colour within
              one pixel of it (in either direction). What antialiasing does differently goes away
              here; a shape that is missing, in another colour or in another place stays.

    compare-pictures.py <directory A> <directory B> [--out <directory>] [--threshold 16]
                        [--worst 12] [--csv <file>]

Pictures are paired by file name (*.png). With --out, the worst pairs by `misplaced` are written
side by side (A, B, and the pixels that stay different in red on a grey copy of A).

Needs numpy and Pillow.
"""
import argparse
import csv
import pathlib
import sys

import numpy as np
from PIL import Image


def load(path):
    return np.asarray(Image.open(path).convert("RGB"), dtype=np.int16)


def near(a, b, threshold):
    """Per pixel: a and b differ by no more than the threshold in every channel."""
    return (np.abs(a - b) <= threshold).all(axis=2)


def has_neighbour(a, b, threshold):
    """Per pixel of a: b has a close colour at the pixel or at one of the eight around it."""
    height, width, _ = a.shape
    padded = np.pad(b, ((1, 1), (1, 1), (0, 0)), mode="edge")
    found = np.zeros((height, width), dtype=bool)
    for dy in range(3):
        for dx in range(3):
            found |= near(a, padded[dy:dy + height, dx:dx + width], threshold)
    return found


def compare(a, b, threshold):
    different = ~near(a, b, threshold)
    misplaced = ~(has_neighbour(a, b, threshold) & has_neighbour(b, a, threshold))
    return different, misplaced


def side_by_side(a, b, misplaced, path):
    grey = (a.astype(np.float32).mean(axis=2, keepdims=True) * 0.5 + 127).repeat(3, axis=2).astype(np.uint8)
    grey[misplaced] = (255, 0, 0)
    sheet = np.concatenate([a.astype(np.uint8), b.astype(np.uint8), grey], axis=1)
    Image.fromarray(sheet).save(path)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("a", type=pathlib.Path)
    parser.add_argument("b", type=pathlib.Path)
    parser.add_argument("--out", type=pathlib.Path)
    parser.add_argument("--threshold", type=int, default=16)
    parser.add_argument("--worst", type=int, default=12)
    parser.add_argument("--csv", type=pathlib.Path)
    args = parser.parse_args()

    names = sorted(p.name for p in args.a.glob("*.png") if (args.b / p.name).exists())
    only_a = sorted(p.name for p in args.a.glob("*.png") if not (args.b / p.name).exists())
    only_b = sorted(p.name for p in args.b.glob("*.png") if not (args.a / p.name).exists())
    if not names:
        print("no picture of the same name in both directories", file=sys.stderr)
        return 2

    rows = []
    for name in names:
        a = load(args.a / name)
        b = load(args.b / name)
        if a.shape != b.shape:
            print(f"{name}: sizes differ {a.shape[1]}x{a.shape[0]} and {b.shape[1]}x{b.shape[0]}", file=sys.stderr)
            continue
        different, misplaced = compare(a, b, args.threshold)
        rows.append((name, float(different.mean()), float(misplaced.mean())))

    rows.sort(key=lambda row: row[2], reverse=True)
    if args.csv:
        with open(args.csv, "w", newline="") as out:
            writer = csv.writer(out)
            writer.writerow(["picture", "different", "misplaced"])
            writer.writerows(rows)

    def spread(index):
        values = np.array([row[index] for row in rows]) * 100
        return (f"median {np.median(values):.3f}%  90% {np.percentile(values, 90):.3f}%  "
                f"99% {np.percentile(values, 99):.3f}%  max {values.max():.3f}%")

    print(f"pairs {len(rows)}, only in A {len(only_a)}, only in B {len(only_b)}, threshold {args.threshold}")
    print(f"different  {spread(1)}")
    print(f"misplaced  {spread(2)}")
    for name, different, misplaced in rows[:args.worst]:
        print(f"  {name}  different {different * 100:.3f}%  misplaced {misplaced * 100:.3f}%")

    if args.out:
        args.out.mkdir(parents=True, exist_ok=True)
        for name, _, _ in rows[:args.worst]:
            a = load(args.a / name)
            b = load(args.b / name)
            _, misplaced = compare(a, b, args.threshold)
            side_by_side(a, b, misplaced, args.out / name)
    return 0


if __name__ == "__main__":
    sys.exit(main())
