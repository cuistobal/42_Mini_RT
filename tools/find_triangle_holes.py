#!/usr/bin/env python3
import sys
from collections import defaultdict, deque

if len(sys.argv) < 2:
    print("Usage: find_triangle_holes.py scene.rt")
    sys.exit(2)

infile = sys.argv[1]

# normalize coordinates to a string key with limited precision
def key_of(v):
    return f"{v[0]:.6f},{v[1]:.6f},{v[2]:.6f}"

triangles = []
edge_count = defaultdict(int)
edge_to_tri = defaultdict(list)

with open(infile, 'r') as f:
    for line in f:
        s = line.strip()
        if not s or s.startswith('#'):
            continue
        if s.startswith('tr '):
            parts = s.split()
            if len(parts) < 5:
                continue
            v0 = tuple(map(float, parts[1].split(',')))
            v1 = tuple(map(float, parts[2].split(',')))
            v2 = tuple(map(float, parts[3].split(',')))
            triangles.append((v0, v1, v2))

# build edge map
for idx, (v0, v1, v2) in enumerate(triangles):
    verts = [v0, v1, v2]
    for i in range(3):
        a = key_of(verts[i])
        b = key_of(verts[(i+1)%3])
        edge = tuple(sorted((a,b)))
        edge_count[edge] += 1
        edge_to_tri[edge].append(idx)

# boundary edges are edges used exactly once
boundary_edges = [e for e,c in edge_count.items() if c == 1]
print(f"Total triangles: {len(triangles)}")
print(f"Total edges: {len(edge_count)}")
print(f"Boundary edges (usage==1): {len(boundary_edges)}")

# build adjacency from boundary edges
adj = defaultdict(list)
for a,b in boundary_edges:
    adj[a].append(b)
    adj[b].append(a)

# find connected boundary loops
visited = set()
loops = []
# Use BFS/connected components on the boundary adjacency to collect loops/components
for start in adj:
    if start in visited:
        continue
    q = deque([start])
    comp = []
    visited.add(start)
    while q:
        u = q.popleft()
        comp.append(u)
        for w in adj[u]:
            if w not in visited:
                visited.add(w)
                q.append(w)
    loops.append(comp)

print(f"Found {len(loops)} boundary loops")
for i,loop in enumerate(loops):
    print(f"Loop {i}: {len(loop)} vertices")
    for key in loop[:10]:
        print("  ", key)
    if len(loop) > 10:
        print("  ...")

# Optionally, write boundary vertices to a file for inspection
with open(infile + ".holes.txt", 'w') as out:
    out.write(f"Total triangles: {len(triangles)}\n")
    out.write(f"Boundary edges: {len(boundary_edges)}\n")
    out.write(f"Boundary loops: {len(loops)}\n")
    for i,loop in enumerate(loops):
        out.write(f"Loop {i}: {len(loop)} vertices\n")
        for key in loop:
            out.write(key + "\n")

print(f"Wrote {infile}.holes.txt")
