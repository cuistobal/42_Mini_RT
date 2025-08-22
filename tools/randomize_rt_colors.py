#!/usr/bin/env python3
import sys
import random

if len(sys.argv) < 3:
    print("Usage: randomize_rt_colors.py input.rt output.rt")
    sys.exit(2)

infile = sys.argv[1]
outfile = sys.argv[2]

GROUP_SIZE = 50

changed = 0
tr_count = 0
current_color = None

with open(infile, 'r') as inf, open(outfile, 'w') as outf:
    for line in inf:
        stripped = line.lstrip()
        if stripped.startswith('tr '):
            # new color at every GROUP_SIZE-th triangle (including first)
            if tr_count % GROUP_SIZE == 0:
                r = random.randint(0, 255)
                g = random.randint(0, 255)
                b = random.randint(0, 255)
                current_color = f"{r},{g},{b}"
            # preserve leading whitespace
            prefix_len = len(line) - len(stripped)
            prefix = line[:prefix_len]
            parts = stripped.rstrip('\n').rsplit(' ', 1)
            if len(parts) == 2:
                geom, _ = parts
                outf.write(f"{prefix}{geom} {current_color}\n")
                changed += 1
                tr_count += 1
                continue
        outf.write(line)

print(f"Wrote {outfile} (modified {changed} 'tr' lines in groups of {GROUP_SIZE})")
