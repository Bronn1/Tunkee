# Tunkee
Port of board turn-based tactical game with armored vehicles theme.
Currently under development.

## What is done?
+ Hexagonal board with simple terrain(grass) and pathfinding module. 
+ Units with abilities to move, shoot.
+ Extendable units damage system.
+ 2D graphics for units and board.
+ Animation support(Burning, Explosion, Shooting).
+ Different unit tooltips with information about internal state.
+ Almost full support for the rules of the game.

## Build
Latest C++20 version of compiler is required.

1. Download and unpack google test framework, Asio, Cereal.
Download and unpack SFML library for your version of compiler(tested with Visual C++ 15 (2017) - works with newest VS compiler):
```
https://github.com/google/googletest/archive/refs/heads/master.zip
https://github.com/USCiLab/cereal/archive/refs/heads/master.zip
https://github.com/chriskohlhoff/asio/archive/refs/heads/master.zip
https://www.sfml-dev.org/download/sfml/2.5.1/
```

2. Change paths in CMakeList.txt to your downloaded gtest, Asio, Cereal, SFML:

```
set(GOOGLETEST_PROJECT_LOCATION D:/path-to-googletest-root-dir)
set(SFML_ROOT D:/path-to-SFML-2.5.1-root-dir)
set(ASIO_ROOT D:/path-to-asio-root-dir/asio)
set(CEREAL_ROOT D:/path-to-cereal-root-dir)
```

3. Standard steps with 'cmake -A Win32 -S .  -B "out/build32"' and  'cmake --build out/build32 --config Debug'.
