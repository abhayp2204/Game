# Zombie Apocalypse

## Install these libraries (instructions for ubuntu)

```
sudo apt-get update
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

## Instructions to test

```
mkdir build
cd build
cmake ..
make -j
```

## Description

This is a simple 2D top down maze game created using OpenGL. The player has to navigate a maze riddled with zombies. Each maze has coins that upon collecting, will increase the score of the player. The goal is simple! Reach the door at every maze without getting eaten by the zombies.
The player has the option of playing in the dark which will boost his/her score.

## Legend

- Green: Player
- Red: Zombies
- Blue: Door
- White: Final Door

## Controls

- WASD - to move up, left, right and down
- L    - toggle lights
- Esc  - exit game