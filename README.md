
## Virtual Alcohol Gallery

  Virtual Alcohol Gallery is a simple visualization project using c++ and opengl.
  
## TODO
List of features left to implement:
 - Fractals;
 - Various visual effects caused by drinking alcohol;
 - Picking up bottles (ray-casting);
 - Animation of drinking;

## Installation & Compilation

This project has following dependencies:
- [glfw](https://github.com/glfw/glfw)
- [glew](https://github.com/nigels-com/glew)
- [assimp](https://github.com/assimp/assimp)
- [glm](https://github.com/g-truc/glm)
- [lodepng](https://github.com/lvandeve/lodepng)

---

To compile this project you need to install the following dependencies: \
 - Arch Linux
```bash
yay -S glfw glew glm assimp
```
 - Debian
```bash
```
**TODO:
Add  dependencies installation for Debian, Fedora etc.**

---

Install the repository:
```bash
git clone https://github.com/n0f1ndv/virtual_alcohol_gallery
```
Compile it using cmake:
Generate build files using:
```bash
cmake -S . -B build
```

 Build project using:
```bash
cmake --build build
```

Clean the project using:
```bash
rm -rf build
```

## Usage & Quick Start
Run the executables with:
```bash
./build/app
```

Controls: \
**TODO:
Add controls**
