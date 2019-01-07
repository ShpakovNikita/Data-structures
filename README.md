# Custom data structures #
## Rss reader and storage for personal info ##
This repository contains implementations and tests of the following data structures:
## Vector ##
This is the basic template vector that was designed to be as std::vector. Also you may find tests for it in vector_test.cpp file.
## Memtree ##
This is the custom and not template implementation of the B tree for. It is not designed to use external memory. You may find test for it in memtree_test.cpp file.

### How to run tests? ###
To run test you need to compile application and run it (tests for each data structure are already included in the main function). To build it you need type the following commands:
```bash
$ cmake
$ make
```
Or this if you have MinGW compiler:
```bash
$ cmake
$ mingw32-make
```
