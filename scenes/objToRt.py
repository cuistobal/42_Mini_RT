
def obj_to_rt(obj_path, rt_path, color="255,255,255"):
    vertices = []
    triangles = []
    with open(obj_path, "r") as f:
        for line in f:
            if line.startswith("v "):
                parts = line.strip().split()
                vertices.append(tuple(map(float, parts[1:4])))
            elif line.startswith("f "):
                parts = line.strip().split()
                idx = [int(p.split('/')[0]) - 1 for p in parts[1:4]]
                triangles.append(idx)
    with open(rt_path, "w") as f:
        for tri in triangles:
            v1, v2, v3 = [vertices[i] for i in tri]
            f.write(f"tr {v1[0]},{v1[1]},{v1[2]} {v2[0]},{v2[1]},{v2[2]} {v3[0]},{v3[1]},{v3[2]} {color}\n")

# Usage

obj_to_rt("dragon.obj", "dragon.rt")