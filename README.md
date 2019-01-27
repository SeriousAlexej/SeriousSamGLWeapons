# SeriousSamGLWeapons

<p align="center">
  <img src="https://raw.githubusercontent.com/SeriousAlexej/SeriousSamGLWeapons/master/Screen/pew.png">
</p>

Technical demo of using OpenGL 3.3 shaders in vanilla Serious Sam game (for weapon models rendering)

This is a small modification for vanilla *Serious Sam The Second Encounter v1.07*
Which uses OpenGL 3.3 shaders for rendering Laser weapon.

Obviously, it does not work on DirectX, but thats ok, there is no use in supporting
dead technologies anyway :)

# INSTALLATION
Binary release can be found here:
https://drive.google.com/file/d/1ztvg1Vm-V9JdbagoWRqruGoXrvhUsztR/view?usp=sharing

Unpack in root directory of your Serious Sam.
You might want to make a backup of your *Classes* folder, if you have it unpacked.
This mod changes *Classes/Player.ecl* file.

# COMPILING
Sources here are provided with Visual Studio 6.0 project files, but the sources itself are
compatible and tested with Visual Studio toolchain v140 (2015).

## WARNING
This mod is not well tested, but has been confirmed to work on multiple NVIDIA cards.

## License
This modification uses GPL v3, for more info, read file LICENSE

## 3rd Party notice
glew - Modified BSD license
glm - Modified MIT license
SeriousEngine - GPL v2 license
stb_image - public domain