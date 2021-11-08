# Tunkee
Turn-based tactical game with armored vehicles theme

## Build
Latest C++20 version of compiler is required.

1. Download and unpack google test framework:
```
https://github.com/google/googletest/archive/refs/heads/master.zip
```
2. Download and unpack SFML library for your version of compiler(tested with Visual C++ 15 (2017) - 32-bit and this version works with newest VS compiler):
```
https://www.sfml-dev.org/download/sfml/2.5.1/
```
3. Change paths in CMakeList.txt to your downloaded gtest and SFML:

```
set(GOOGLETEST_PROJECT_LOCATION D:/path-to-googletest)
set(SFML_ROOT D:/path-to-SFML-2.5.1)
```
4. Standard steps with 'cmake && cmake --build . --config Debug'. Should be built for x86 version