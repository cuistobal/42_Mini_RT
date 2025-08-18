MiniRT
======

MiniRT is a minimalist raytracer project developed at 42. It generates realistic 3D images from scenes described in text files, using acceleration techniques like BVH and multi-threading.

Main Features
-------------
- Supported primitives: sphere, plane, cylinder, cone, cube
- Multiple light sources and ambient lighting
- Different materials with their own color, reflection, transparency and refraction index
- Camera with movements (translation, rotation, zoom)
- Acceleration structure (Bounding Volume Hierarchy with Surface Area Heuristic)
- Dynamic multi-threading (thread pool && block-based rendering)
- Parsing of complex scenes (.rt files)
- Shadows, reflections, refractions
- Using 42 MiniLibX for graphical display && events management
- Profiling

- Currently working on textures (chessboard && bump maps) 

Installation
------------
1. Clone the project:
	```sh
	git clone --recursive https://github.com/<your_user>/42_Mini_RT.git
	cd 42_Mini_RT
	```
2. Install system dependencies (Ubuntu):
	```sh
	sudo apt-get update && sudo apt-get install -y build-essential libx11-dev libxext-dev libxpm-dev libxrandr-dev libbsd-dev libgl1-mesa-dev libglu1-mesa-dev x11-apps git make
	```
3. Compile MiniLibX (if not done automatically):
	```sh
	make -C minilibx-linux
	```
4. Compile the project:
	```sh
	make
	```

5. In case you have a submodule problem:
	```sh
	git submodule update --init --recursive
	```

6. Or want the latest versions of submodules:
	```sh
	git submodule update --remote --merge
	```

Usage
-----
Run the raytracer with a scene:
```sh
./miniRT scenes/test.rt
```

We use multithreading for the redering. It is possible to define the NUM_THREAD variable at compile time. One further improvment of our project would be to detect the number of cores && available threads and set
this limit dynamically.

Keyboard/mouse controls (examples):
- WASD / arrow keys: move the camera
- Mouse wheel: zoom in/out
- ESC: quit

We're currently working on implementing different movements for WASD && arrow keys to improve UI. Also, we
might implement controller support.

Creating a Scene
----------------
Scene files (.rt) describe objects, camera, and lights. Example:
```
A 0.2 255,255,255
C 0,0,0 0,0,1 70
L 0,10,0 0.7 255,255,255
pl 0,-5,0 0,1,0 100,100,100
sp 0,0,10 5 255,0,0
cy 10,0,10 0,1,0 2 8 0,255,0
```

One further improvment would be to support triangle, because the dragon && wolf scenes looked dope on the repo where we foudn them.

Profiling & Optimization
------------------------
To compile with profiling support:
```sh
make profile
```
After running, get a quick performance recap && analyze the results:
```sh
make analyze
```

Or the following command to get a full profiling log:
```sh
gprof miniRT gmon.out > profiling.log
```

Notes
-----
- The project uses MiniLibX as a git submodule.
- For graphical display in Docker, see the Dockerfile and documentation.
- For questions or bugs, contact the author.
