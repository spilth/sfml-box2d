# SFML & Box2D

Adapted from [Tutorial: Getting Started with Box2D and SFML](https://veendeta.wordpress.com/2012/02/16/tutorial-getting-started-with-box2d/)

## Getting Started

### Install Prerequisites

You'll need the following installed:

- [SFML](https://www.sfml-dev.org)
- [Box2D](https://box2d.org)
- [CMake](https://cmake.org)

You can install them easily with [Homebrew](https://brew.sh):

```bash
$ brew install sfml box2d cmake
```

### Checkout, Build, Run

```bash
$ git clone https://github.com/spilth/sfml-box2d
$ cd sfml-box2d
$ mkdir cmake-build-debug
$ cd cmake-build-debug
$ cmake ..
$ cmake -build .
$ cd ..
$ ./cmake-build-debug/sfml-box2d
```

Note: If you import the project into CLion, be sure to set `sfml-box2d | Debug`'s working directory to the root of the project.

## Screenshot

![Screenshot](screenshot.png)