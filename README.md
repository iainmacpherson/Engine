# Engine

_Note: this is a small project where I mess around with graphics programming, in particular using OpenGL. My goal was to learn more about graphics programming libraries and to develop an appreciation for the basics of computer graphics._

_This project draws heavily from [Learn OpenGL](https://learnopengl.com/)._

## Requirements
#### 3rd Party Libraries
* GLFW
* GLAD
* GLM
* STB Image

#### Setup
1. Download the library headers and place them in a new folder called "include".
2. Place the library objects in a new folder called "libs".
3. Check the CMakeFiles.txt to ensure that the library objects are correctly linked.

## Building the project
1. Create a "build" directory.
2. `cd build`
3. `cmake ..`
4. `cmake --build .`

## TODO
* Model loading
