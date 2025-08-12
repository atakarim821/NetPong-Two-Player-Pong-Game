# Ping Pong Game

This is a simple command-line Ping Pong game implemented in C. The project demonstrates basic game logic, networking (for multiplayer capability), and modular code organization.

## Features

- Two-player ping pong gameplay
- Networking support to play over LAN
- Modular design with separate files for game logic and networking
- Easy to build and run using a Makefile

## Directory Structure

```
.
├── include
│   ├── game.h
│   └── net.h
├── Makefile
├── pingpong
├── README
└── src
    ├── game.c
    ├── net.c
    └── pingpong.c
```

## Building

To build the project, simply run:
```sh
make
```

## Running

To start the game after building:
```sh
./pingpong
```

Follow the on-screen instructions to start a server or connect as a client.

## Requirements

- GCC compiler
- Make

## How it works

- The game logic is implemented in `game.c` and `game.h`.
- Networking functions are in `net.c` and `net.h`.
- The main entry point is `pingpong.c`.

## Contributing

Feel free to open issues or submit pull requests to improve the game!

