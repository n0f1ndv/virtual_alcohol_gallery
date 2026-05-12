
## Virtual Alcohol Gallery

  Virtual Alcohol Gallery is a simple visualization project using c++ and opengl.
  
## TODO
There's a funny bug:  when window is created it has certain size but when it is updated class member is not changed and postprocessing class also is required to keep track of size but it get segmentation fault when I try to fix it.
Everything is fine as long as you don't change screen size. Damn, we need to fix this.

List of features left to implement:
 - Various visual effects caused by drinking alcohol;
 - Picking up bottles (ray-casting);
 - Animation of drinking;
 - Constant frame rate.

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
