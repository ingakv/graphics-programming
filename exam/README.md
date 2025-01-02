# PROG2002 Exam Autumn 2023

**Sources:** All code used has been made by me during the semester and was submitted with the assignment earlier this semester.

[Video showcase](https://youtu.be/LKWzwWWz0bw)


## Task 1

For the geometry, I used the framework that I made myself.
I used this to create unit grids and unit cubes.
Because I implemented topologies for the shapes, the vertices are not rendered several times,
since they are rendered using indexes rather than bigger arrays.

I made the grid using the framework, but I applied the checkered pattern in the shader.
(The program can still identify the different tiles inside the grid.)
In the shader it calculates what index (0 - 10*10) the shader is currently on based on the position vector.
Through a while loop which subtracts a certain number for each iteration, it calculates what tile it is on
(x = 0–9, y = 0–9).
Based on the tile index, it is able to alternate between the two colors in a way that makes the grid checkered.

The cube shaders use a similar system in the shader, but for their position on the board instead of the color.
In the app loop, using two for loops, all the tiles of the grid are checked.
If a tile is not empty, the tile's index, and what object it is that is occupying it, is sent to the shader.
Based on the index, the cube gets placed correctly (position - tile_width * (board size-1)).

In order to differentiate between boxes, the player, the target locations,
and walls, I made a sort of enumerator in the shader.
I made a uniform int variable that is sent to the shader before every object is drawn.
This number goes from 0 to 3.
That way I don't have to use different shaders for each of the objects,
and I can instead easily just change the color of the object depending on what the "enumerator" value is.

The player is represented by a pink box that is slightly smaller than the rest.

The boxes are yellow.

The target locations are green and more transparent than the others.

When a box is placed in a target location, the color of the box / target location turns to opaque

The wall blocks are dark-blue.

---

## Task 2

The board is a two-dimensional array consisting of Object instances.
Objects can be Player, Box (, Target), Wall, or Empty.
When an array key is pressed, the player instance is moved to the new correct index in the array if that index is Empty.
Targets have a separate board, which allows all other objects on the board to move independently of the targets.

When a box has been placed on a target tile, the box will turn green,
similar to the target's color, but with full opacity.

---

## Task 3

I used the texture manager framework that I made for the assignment.
By doing this, the textures are bound to the shaders.
I retrieved the textures
by using a function which finds the correct textures through the GetUnitByName function within the texture manager.
The textures are applied in the shaders where they are mixed 50% with the objects' color.

---

## Task 4

In order to make the Phong lighting module to act correctly,
I had to change my unit cube topology to include all the normal vectors,
which I had done incorrectly previously.
In the shader, I combine the color and brightness of the ambient, diffuse, and specular light.
These get combined in the fragment shader by using different formulas
and create an effect which simulates light being shined onto the cubes,
with the light source being the camera position.

---

## Task 5

I added smooth movement by implementing a movement vector.
Each time a cube is moved, their previous location and the timestamp of the movement are added to the vector.
If a cube is in the middle of a movement,
their position will be slightly altered frame by frame until they are in their new location.
The movement interpolation is calculated with the sine of time between the current time and what the time was when the movement was started.

By using the same type of interpolation using the time,
the player cube's brightness changes depending on the sine of the time passed.
