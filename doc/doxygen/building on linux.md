Building on Linux
----------------

## Tip: Development build

If you just want to run RoR without any hassle then have a look at the development
builds: https://forum.rigsofrods.org/threads/ror-development-builds-for-0-4-8-for-windows-and-linux.696/

# Bulding with conan

## Introduction

Rigs of Rods is very complex and is built on top of many components to provide its features. This results in a long list
of dependencies. Most dependencies can usually be found in the software repositories of your distribution but some have
to be compiled by hand. Fear not! While this may look overwhelming at first it is actually very easy if you follow these
steps closely. <br>

## Building Rigs of Rods

Download the source code:

``` bash
cd ~/
git clone --recursive https://github.com/RigsOfRods/rigs-of-rods.git
cd rigs-of-rods
```

## Configuring your build

Before you can compile the code you have to configure your build by running cmake. cmake automatically detects what
platform you are using, finds the dependencies, sets up the flags for the compiler accordingly etc..

``` bash
cmake -GNinja -DCMAKE_BUILD_TYPE=Release .
```

## Building the source code

To start building simply run:

``` bash
ninja
```

Thats it! You finally built Rigs of Rods.

## Updating existing sources

If you already got the sources and just want to update, follow the steps below:

``` bash
cd rigs-of-rods
git pull

cmake .
ninja
```

# Bulding without conan

First install nvidia-cg-toolkit

then we need to get dependencies compiled

```
git clone https://github.com/RigsOfRods/ror-dependencies
cd ror-dependencies
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo .
make -j8
```

Αfter that we need to clone ror it self

```
git clone --recursive https://github.com/RigsOfRods/rigs-of-rods
cd rigs-of-rods
cmake -DCMAKE_PREFIX_PATH=~/ror-dependencies/Dependencies_Linux/ -DCMAKE_BUILD_TYPE=RelWithDebInfo -DUSE_PACKAGE_MANAGER=OFF .
make -j8
```

and thats it!
