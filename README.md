# Graphics Project
This repo contains all files for Alastair Hamilton's (2503450)
graphics project for course CMP502.

# Compiling
## Prerequisites
- Windows (tested on Windows 10)
- [CMake](https://cmake.org/download/)
- [Ninja](https://ninja-build.org/)
- [DirectX11](https://support.microsoft.com/en-gb/topic/how-to-install-the-latest-version-of-directx-d1f5ffa5-dae2-246c-91b1-ee1e973ed8c2)
- [Microsoft Visual Studio C++ Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019)

## Build and Compile
In the root directory:
```bash
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Debug
ninja -C build
```

## Run
GraphicsProject.exe will now be available in `build/bin`:

```bash
.\build\bin\GraphicsProject.exe
```

# Author
Alastair Hamilton (2503450), 2503450@uad.ac.uk

# References
References are included below. Where applicable it has been mentioned
when code from these contributors has been used.

* Art-Michel (2025) Github. Available at: https://github.com/Art-Michel/Flowerwall-CRT-shader-for-Godot/blob/main/addons/flowerwall_postprocess/shaders/crt.gdshader (Accessed: November 2025).
* ArtStation (n.d.) Pinterest. Available at: https://uk.pinterest.com/pin/texturelow-poly-game-water001-cgshare-book-on-artstation-at-httpswwwartstationcomartworkzax5bl--702209766878021267/ (Accessed: November 2025).
* berwyn (2016) Stack Overflow. Available at: https://stackoverflow.com/questions/34886497/how-to-share-a-struct-between-c-directx-and-hlsl (Accessed: November 2025).
* Falconer, R. (n.d.) Graphics Lab Code. (Accessed: 2025).
* Flaticon (n.d.) Flatoicon. Available at: https://www.flaticon.com/free-icon/flashlight_4151169 (Accessed: November 2025).
* Hughes, E. (n.d.) Graphics Lab Code. (Accessed: 2025).
* ImGui Contributors (2025) Github. Available at: https://github.com/ocornut/imgui (Accessed: November 2025).
* Luna, F.D. (2012) Introduction to 3d game programming with Directx11. Dulles: Mercury Learning and Information.
* Mark Finch, C.W. (2004) Nvidia GPU Gems. Available at: https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models (Accessed: 09 November 2025).
* Microsoft Copilot (2025) Microsoft Copilot. Available at: https://copilot.microsoft.com/ (Accessed: 09 December 2025).
* Motion Forge Pictures (2021) Motion Forge Pictures. Available at: https://www.motionforgepictures.com/height-maps/ (Accessed: November 2025).
* Robertson, P. (n.d.) DXFramework. (Accessed: 2025).
* Tardif, A. (n.d.) Alex Tardif. Available at: https://alextardif.com/Water.html (Accessed: 9 December 2025).
* Walbourn, C. (2022) Stack Overflow. Available at: # https://stackoverflow.com/questions/71299716/how-to-compile-hlsl-shaders-during-build-with-cmake (Accessed: November 2025).
