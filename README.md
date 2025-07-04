Work in progress

We're implementing a Ray tracer in C using minilibx (https://github.com/42Paris/minilibx-linux), a simple X-Window (X11R6) programming API in C, designed for students, suitable for X-beginners.

Ray tracing is a rendering technique used in computer graphics to generate highly realistic images. It is used in Computer Generated Images, so you could find it in your favourite video game or movie.
The difference with ray casting is that it simulates interaction objects with lights, based on the relative position of the camera on the scene depicted.

The algorithm casts ray from the viewer's eye to a pixel on the scene. If the ray intersects with an object, the casting algorithm is called recursively to assess interaction with various surfaces and lights.
Depending on the scenario, the object's color will be either returned as is or tinted. The tint is due to the object's light source being hidden by another object.

![shadows](https://github.com/user-attachments/assets/ee537f9f-ac8d-43eb-9b63-c0cdfa379440)
(Image generated with Mistral AI's - le chat)

In the mandatory part, we don't bother with material types and reflection/refraction. But in real life, such surfaces can refract && reflect light.
Water is a good example of doing both, explaining why what you see though water depicts what's under it but with an angle (think about the way your legs look when you're 5 feets in a crystal clear lake).
Yet the surface of the lake could also reflect a distant object, such as a moutain / forest / house behind it:

/*
  INSERT IMAGE EXAMPLE
*/

![reflexion](https://github.com/user-attachments/assets/b877cb73-a383-487b-ba4a-a4e47aafd0b8)
(Image generated with Mistral AI's - le chat)

![refraction](https://github.com/user-attachments/assets/9c6bc177-0e42-48e1-98da-94b6627e1e26)
(Image generated with Mistral AI's - le chat)

Since we have to cast a lot of rays from a lot of pixels, it's impossible to get a fluid real time rendering result without a few optimizations. First, you could restrict the research area for each ray using a tree
structure called BVH. Also, you can limit the recursion depth for each ray to 3 to 5 calls. That means trading quality of shading/colors for quantity of FPS. Finally, you could use multithreading and shift some small
calculs from the CPU to the GPU.

//
Boundaries Volume Hierarchy using a Surface Area Heuristic aka BVH with SAH :

So you have this x,y,z scene, with say 5 objects, all of which are concentrated in the upper right of the scene. Why casting rays to the other parts of the scene then ?
That's why BVH exist, and as we understand it, they belong to the familly of BST, hence they need to be balanced for efficiency purposes.

The idea is to encapsulate object into boxes that get recursively smaller, untill you meet you max BVH depth / SAH (it's up to you basically). 

That means, for example if you set an Heuristic to value x, the tree building algorithm will recursively split the bigger boxes into smaller ones untill all the smol boxes
have a size <= to the SHA. That means you'll cast rays very efficiently.
Bottleneck -> If you have a big enough discrepency between the size of your objects and you SAH, your tree's depth is going to skyrocket.

On the other hand, you could use a Max BVH depth. That means your boxes might get a bit bigger, reducing both the ray casting's efficiency && the cost of building the BVH.

In the context of a fixed scene like ours, we don't have to rebuild the BVH constantly as object don't appear/disapear/move. But if you're building a game engine, the strategy
you'll implement for that purpose sounds quite critical. They are a few ways of doing it and we aon't delve into them here as it's not the educational purpose of this project.

//
The multi threaded approach:

As the results of each ray casted is independent from other rays, multithreading the computation of each ray sounds like a pretty good optimization solution. And that comes at the
cost of concurency handling, aka data races.


//
Current state of the project:

Now that the basics work, we want to implement a functional BVH tree with a SAH for real time rendering. As we're not building a game engine, implementing collisions/interractions
are not part of our focus right now.
