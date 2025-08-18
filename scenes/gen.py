import math
import random

with open("dragon_cartoon.rt", "w") as f:
    # Ambiance, caméra, lumière
    f.write("A 0.2 255,255,255\n")
    f.write("C 0,0,12 0,0,-1 60\n")
    f.write("L 10,10,10 0.8 255,255,255\n\n")

    # Corps sinueux
    n_body = 1200
    for i in range(n_body):
        t = i / n_body * 6 * math.pi
        x = t * math.cos(t) * 0.7
        y = t * math.sin(t) * 0.7
        z = 0.3 * math.sin(2 * t)
        r = 0.5 + 0.3 * math.sin(t + i/50)
        color = f255,{80 + int(40*math.sin(t))},20\
        f.write(f\"sp {x:.3f},{y:.3f},{z:.3f} {r:.3f} {color}\\n\")

    # Queue
    for i in range(200):
        t = i / 200 * math.pi
        x = -1.5 - t * 0.7
        y = -1.5 - t * 0.5
        z = -0.5 + 0.2 * math.sin(t)
        r = 0.25 - 0.15 * (i/200)
        f.write(f\"sp {x:.3f},{y:.3f},{z:.3f} {r:.3f} 255,80,20\\n\")

    # Tête (amas sphères)
    for i in range(200):
        phi = random.uniform(0, 2*math.pi)
        costheta = random.uniform(-1, 1)
        u = random.uniform(0, 1)
        theta = math.acos(costheta)
        r = 0.7 * (u ** (1/3))
        x = 4.5 + r * math.sin(theta) * math.cos(phi)
        y = 4.5 + r * math.sin(theta) * math.sin(phi)
        z = 2.2 + r * math.cos(theta)
        f.write(f\"sp {x:.3f},{y:.3f},{z:.3f} {0.18 + 0.12*u:.3f} 255,80,20\\n\")

    # Cornes
    for i in range(40):
        angle = i / 20 * math.pi
        x = 5.2 + 0.5 * math.cos(angle)
        y = 5.2 + 0.5 * math.sin(angle)
        z = 2.7 + 0.2 * i/40
        f.write(f\"sp {x:.3f},{y:.3f},{z:.3f} 0.09 255,200,100\\n\")

    # Yeux
    f.write(\"sp 5.2,5.0,2.5 0.08 0,0,0\\n\")
    f.write(\"sp 4.8,5.0,2.5 0.08 0,0,0\\n\")

    # Pattes (amas sphères)
    for j in range(4):
        base_x = [2.0, 3.5, 0.5, 1.5][j]
        base_y = [1.0, 2.5, -0.5, 0.5][j]
        base_z = [-0.8, -0.8, -0.8, -0.8][j]
        for i in range(40):
            phi = random.uniform(0, 2*math.pi)
            costheta = random.uniform(-1, 1)
            u = random.uniform(0, 1)
            theta = math.acos(costheta)
            r = 0.3 * (u ** (1/3))
            x = base_x + r * math.sin(theta) * math.cos(phi)
            y = base_y + r * math.sin(theta) * math.sin(phi)
            z = base_z + r * math.cos(theta)
            f.write(f\"sp {x:.3f},{y:.3f},{z:.3f} {0.09 + 0.07*u:.3f} 255,80,20\\n\")
