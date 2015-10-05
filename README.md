# Machines and Computability project
A universal Turing machine simulator.
This program was written in C++ as part of a Machines and Computability course at the [University of Antwerp](https://www.uantwerpen.be/popup/opleidingsonderdeel.aspx?catalognr=1001WETCGR&taal=en&aj=2014).<br>

copy other reame file 
fill in soon

![alt text](https://raw.githubusercontent.com/arminnh/Basic-Graphics-Engine/master/images/spheres_and_cylinders/spheres_and_cylinders014.png)



###Features
* 2D L-systems
* Linedrawings
* 3D figures
* Z buffering with lines and with triangles

 #####pdfs for implementing features
    * 3D fractals
    * lighting
        * Ambient
        * Diffuse
        * Specular
    * Shadowing
    * Texture Mapping
    * Spheres and cylinders

###Install
in src:
```bash
make
make clean
```
Engine will be created in the images folder

###Usage
engine [files]

script [options]  
options:
   * all
   * 2dlsystems
   * linedrawings
   * 3dfigures
   * zbufferinglines
   * zbufferingtriangles
   * 3dfractals
   * lightingambient
   * lightingdiffuse
   * lightingspecular
   * spheresandcylinders

###Libraries used
* EasyImage
* ini_configuration
* lparser
* vector3D
Documentation can be found in slides+pdfs/tools/

<br> <br> <br>
####Todo
Finish features + CMake + images script + tests


