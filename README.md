# ChessPlusPlus - C++ Chess Application

![C++ Logo](https://img.shields.io/badge/C%2B%2B-blue.svg)


## Overview

<div style="display: flex; justify-content: space-between; width: 100%;">
    <img src="/img/board.png" style="width: 48%;" alt="Chess board screenshot">
    <img src="/img/move.png" style="width: 48%;" alt="Chess move screenshot">
</div>

ChessPlusPlus is a desktop chess application built using C++ and SDL2. It provides a simple, interactive environment to play chess, leveraging the SDL2 library for graphics and user interaction.

## Features
- Classic chess gameplay
- Interactive user interface with SDL2
- Supports local player vs player

## Installation

Follow the steps below to set up and run ChessPlusPlus on your local machine:

1. Clone the repository.
2. The SDL2 and SDL2_image libraries are already included in the repository. We just need to configure the project settings in Visual Studio:
   - Add the path to the include folders of SDL2 and SDL2_image in the **C/C++ General** settings.
   - Add the path to the lib folders of SDL2 and SDL2_image in the **Linker General** settings.
   - Add the SDL2 and SDL2_image libraries in the **Linker Input** settings.
   - These include and library files are located within the cloned repository's respective SDL folders.
3. Build and run the `Main.cpp` file, and you are ready to go!

## Prerequisites

Before running the application, ensure you have the following installed:

- A C++ compiler (preferably Visual Studio)

## Acknowledgements

I extend my gratitude to the following resources that contributed to the development of this project:

- Chess Design: [Chess Simple Assets](https://www.figma.com/community/file/971870797656870866/chess-simple-assets) by [Maciej Świerczek](https://www.figma.com/@swierq)

## License

This project is licensed under the MIT License - see the LICENSE file for details.
