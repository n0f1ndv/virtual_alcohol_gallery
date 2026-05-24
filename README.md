
## Virtual Alcohol Gallery

  Virtual Alcohol Gallery is a simple visualization project using c++ and opengl.
  This project aims to show some intresting post-processing effects e.g.: fractals lens, color fluctuation and distortion overlay.

  (This project doesn't promote drinking alcohol. Stop drinking, it's unhealthy. Really.)
  
## TODO
List of features left to implement:
 - Constant frame rate;
 - Resizing window (There's a funny bug:  when window is created it has certain size but when it is updated class member is not changed and postprocessing class also is required to keep track of size but it get segmentation fault when I try to fix it. Everything is fine as long as you don't change screen size (temporary fix).)

## Installation & Compilation

This project use following dependencies:
- [glfw](https://github.com/glfw/glfw)
- [glew](https://github.com/nigels-com/glew)
- [assimp](https://github.com/assimp/assimp)
- [glm](https://github.com/g-truc/glm)
- [lodepng](https://github.com/lvandeve/lodepng)

---

This project targets Linux as main platform and it may not work correctly on different Operating Systems.
To compile this project you need to install the following dependencies: \
 - Arch Linux
```bash
yay -S glfw glew glm assimp
```
If you use different linux distro you should check out whether the same packages are available through you distros' package manager.

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
After compiling the project run the executable with:
```bash
./build/app
```

Controls: 
 - WSAD - move around;
 - E - pick up a bottle;
 - C - smoke a cigarette;
 - U - fly.
