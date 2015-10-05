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


# Universal Turing Machine

## Using the program

Start the program by running:
~~~sh
./Tools_UI.bin
~~~
When running this binary, a user interface will begin for full functionality. Please note that you will be required to enter '.xml' filenames, so be sure to remember
their names and locations.
In case a quick test is required, binaries are also available with specific functionality:
For quick parsing + showing a CFG:
~~~sh
./cfgParser.bin [turingmachine.xml]
~~~
For converting a CFG to PDA (and generating a dot file for visual representation):
~~~sh
./cfgToPda.bin [cfg.xml] [pda.dot]
~~~
For converting a CFG in CNF:
~~~sh
./cnfTest.bin [cfg.xml]
~~~
For testing cyk membership:
~~~sh
./cyk.bin [cfg.xml] "string"
~~~
For converting an arbitrary turing machine for use in the UTM:
~~~sh
./tmConverterTest.bin [turingmachine.xml]
~~~
For running a simpel turing machine:
~~~sh
./tmSimulator.bin [turingmachine.xml]
~~~
For running the universal turing machine and pass an arbitrary turingmachine as parameter:
~~~sh
./utmSimulator.bin [universalTM.xml] [turingmachine.xml]
~~~

## Project

UTM is designed as a teaching tool for Context Free Grammars (CFG) and Turing Machines (TM).
The project was implemented using C++ 11 and uses XML files as input.

### PDA
#### Usage
PDA's are are used for graphical representations of CFG's and possibly for debugging purposes.
To graphically represent a PDA, dot files are created. This requires a package like graphviz.
The transition diagram in png format is created by:
@verbatim
dot [automaton.dot] -Tpng -o [automaton.png]
@endverbatim

The resulting image is shown below:
@image html pda.png
@image latex pda.png

#### Algorithms
Only 1 algorithm was implemented for PDA's. This is the conversion between PDA's with acceptance by empty stack
and acceptance by final state.

The resulting image is shown below:
@image html pda1.png
@image latex pda1.png
@image html pda2.png
@image latex pda2.png

## Directory structure

All source files are located under
~~~
src/{RELEVANT_DIRECTORY}/
~~~
All main files are located under
~~~
tools/
~~~
All data that can be used (like xml files) are located under
~~~
data/
~~~
All test files are located under
~~~
test/
~~~
All documentation can be found under:
~~~
doc/
~~~

## Building with CMake

~~~sh
mkdir build && cd build
cmake ..
make
~~~

## Creating the transition diagrams with graphviz

OPM: graphviz must be installed for this part.

Navigeer naar directory met *.dot files.
Om een grafische weergave van de *.dot files te creeren gebruik je volgend commando:

~~~sh
dot 'filename.dot' -Tpng -o 'afbeeldingname.png'

[bv: dot finiteautomaton.dot -Tpng -o afbeelding.png]
~~~

## Creating documentation

OPM: doxygen moet geinstalleerd zijn voor volgend onderdeel
Voor de documentatie in html/latex:
navigeer naar 'doc' directory
gebruik commando:

~~~sh
doxygen
~~~
open file: "html/index.html"

## Tests

For testing the google testing framework was used.

### Running the tests

~~~sh
mkdir build
cd build
cmake ..
make
./runUnitTests
~~~
