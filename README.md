node-esms
=========

This is a node.js port of [esms](http://eli.thegreenplace.net/programs-and-code/esms/)
The original description of esms source code

> A word about this source code:
>
> ESMS was initially written together with a friend as our first real software project. We learned C while writing it, and frankly, this shows on parts of the code. So, the initial version was quite a lopsided lump of C - which nevertheless worked and did its job admirably. In later incarnations, when I rewrote and added features, I switched to C++ and tried to make the code cleaner, adhering to real programming practices. So the source code is a mash-up of older C code and newer C++ code.
>
> Compiling:
>
> All the source code compiles with any ANSI C++ compiler. Use the makefiles to see how to compile it.
>
> License:
>
> LGPL
>
>
> --
> Eli Bendersky (eliben@gmail.com)

##Objectives
Objectives of this port is to allow esms work nicely with node.js based servers

##Compiling
To compile node-esms, First install node.js
 1. Setup environment: sudo apt-get install -y build-essential libssh-dev git-core curl
 2. Download latest node.js source code: wget http://nodejs.org/dist/latest/node-v0.10.19.tar.gz
 3. Decompress node.js: tar -zxf node-v0.10.19.tar.gz
 4. Goto source folder: cd node-v0.10.19
 5. Create makefile: ./configure
 6. Compile: sudo make
 7. Install: sudo make install

Compiling node-esms from source code with [node-gyp](http://nodejs.org/api/addons.html#addons_hello_world)
 1. cd node-esms/
 2. node-gyp configure
 3. node-gyp build
 4. binary of esms can be found at ./src/build/Release/
 5. To test it node tests/test_esms.js

To use binary form, get it from [npm](https://npmjs.org/package/node-esms)
 1. npm install node-esms

##References
* https://developers.google.com/v8/embed?csw=1
* https://developers.google.com/v8/get_started
* http://nodejs.org/api/addons.html#addons_hello_world
* http://eli.thegreenplace.net/programs-and-code/esms/
