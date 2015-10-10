# Machines and Computability project
## Teaching Tools
A program to aid students in the understanding of several concepts seen in machines and computability, mote specifically Context Free Grammars, Turing Machines and Universal Turing Machines.  
The tools were written in C++ as part of a [Machines and Computability course](https://www.uantwerpen.be/popup/opleidingsonderdeel.aspx?catalognr=1001WETCGR&taal=en&aj=2014) at the University of Antwerp.  

![alt text](doc/2015-10-05 20_27_12-eOS Freya 64 - VMware Player (Non-commercial use only).png)

### Features
* Context Free Grammars
 * Convert to PDA
 * Convert to Chomsky Normal Form
 * Do CYK test
 * Write out to XML
* Turing Machines
 * Set input
 * Show state
 * Run
 * Output simulation to file
* Universal Turing Machines
 * Simulate (any) other turing machines

All input files are .xml files.

### Install
in build/
```cmake
cmake ..
make all
```
The tools will be installed in the build folder.

### Usage

Start the main program by running
~~~sh
./Tools_UI.bin
~~~
When running this binary, a user interface will begin for full functionality. Please note that you will be required to enter '.xml' filenames, so be sure to remember their names and locations.

Sample .xml files can be found in build/data/

##### Other binaries with specific functionality:
run tests with google framework
~~~sh
./runUnitTests
~~~
convert a CFG to PDA (generates a dot file for visual representation with graphviz)
~~~sh
./cfgToPda.bin [cfg.xml] [pda.dot]
~~~
convert a CFG to CNF
~~~sh
./cnfTest.bin [cfg.xml]
~~~
test cyk membership
~~~sh
./cyk.bin [cfg.xml] "string"
~~~
run a simple turing machine
~~~sh
./tmSimulator.bin [turingmachine.xml]
~~~
convert an arbitrary turing machine for use in the UTM
~~~sh
./tmConverterTest.bin [turingmachine.xml]
~~~
For running the universal turing machine and pass an arbitrary turingmachine as parameter
~~~sh
./utmSimulator.bin [UTM.xml] [turingmachine.xml]
~~~
quick parse + show a CFG
~~~sh
./cfgParser.bin [cfg.xml]
~~~

### Creating documentation

Requires doxygen to be installed.
in doc/
~~~sh
doxygen
~~~
open html/index.html

### Libraries used
* gtest-1.7.0
* pugixml-1.4

Presentations about the project can be found in information/

### How our Universal Turing Machine works
This is explained in detail in information/README_UTM.txt (in Dutch).
A brief explanation + example of the UTM's algorithm (in English) is available in information/finalPresentation.pdf

### Authors
[Armin Halilovic](https://github.com/arminnh)  
[Josse Coen](https://github.com/jsscn)  
[Bruno de Deken](https://github.com/brunodd)  
[Fouad Kichauat](https://github.com/Fouad-Kichauat)  
