# Sandro Run: A 3D Game Project

Welcome to the repository for "Sandro Run", a 3D game developed as part of the Computer Graphics course at Politecnico di Milano. This project, which achieved a final evaluation of 30 cum laude, is written in C++ and utilizes Vulkan APIs.
This repository contains also the course assignments. Check the `Assignments` folder for more information.

## Game Features

"Sandro Run" boasts a variety of features that contribute to an immersive gaming experience:

- **Multiple Shaders & Pipelines**: Enhance the visual appeal of the game.
- **Splash Screens**: Provide an engaging introduction to the game.
- **Motorbike Main Character**: The player navigates the game world on a motorbike.
- **Dynamic Sky-Boxes**: The game environment changes with three different sky-boxes based on the time of day.
- **Periodic Scenery**: The game world includes a terrain with hills, trees, a street, guard rails, and streetlights.
- **Moving Cars**: Cars move in both directions on the street, adding to the game's challenge.
- **Motorbike Sounds**: Enhance the realism of the motorbike.
- **Two Cameras**: The game can be viewed from a third-person camera or a first-person camera.
- **Collision Detection**: Collisions are detected between the motorbike and the cars, as well as between the motorbike and the guard rails.
- **Score System**: Players can track their progress and aim to improve their scores.
- **HUD Display**: A heads-up display (HUD) shows the player’s score and the velocity of the speedometer.
- **Game Over Screen**: Displays when the game ends.
- **Presentation Mode**: This mode deactivates collision detections and camera limits for a more relaxed gaming experience.

## Installation & Running

### Prerequisites

Before you can run "Sandro Run", you'll need to install some dependencies. For the basic dependencies, please refer to this [website](https://vulkan-tutorial.com/Development_environment).

For the audio dependencies, you'll need to install `libao` and `libsndfile`. On Ubuntu distributions, you can do this with the following command:

```bash
sudo apt install libsndfile1-dev libao-dev
```

### Building the Project

To build the project, run the following commands:

```bash
mkdir build
cd build
cmake ../
make
```

### Running the Project

To run the project, navigate to the `bin` directory in the `build` folder and execute the `Sandro_run` file:

```bash
cd build/bin
./Sandro_run
```

## Licenses

"Sandro Run" uses several 3D models and textures. You can find the sources for these assets below:

- 3D Models:
  - [Motorbike](https://www.cgtrader.com/free-3d-models/vehicle/motorcycle/suzuki-gsx-750-bike)
  - [Guard Rail](https://www.cgtrader.com/free-3d-models/exterior/street-exterior/guardrail-fd10776d-d363-4be6-b5bd-6853aa50c539)
  - [Tree](https://www.cgtrader.com/free-3d-models/plant/leaf/low-poly-plant-set)
  - [Car](https://www.cgtrader.com/free-3d-models/car/car/low-poly-cars-03d6cf0c-0882-44e2-a22c-02d4f8e68eeb)
- Textures:
  - [Road](https://www.sketchuptextureclub.com/textures/architecture/roads/roads/cracks-road-pbr-texture-seamless-21574)
  - [Broken Glass](https://www.hiclipart.com/free-transparent-background-png-clipart-iyijp/)
  - [Splash Screen](https://www.10wallpaper.com/view/Honda_cb1000r-HD_Desktop_Wallpaper.html)
  - Skybox:
    - [Night](https://www.cleanpng.com/png-space-skybox-texture-mapping-cube-mapping-night-sk-776480/)
    - [Day](https://imgbin.com/png/vD943E4H/skybox-texture-mapping-cube-mapping-desktop-png)
    - [Sunset](https://gamebanana.com/mods/7921)
  - Speedometer:
    - [Speedometer](https://freepng.pictures/download/speedometer-19/)
    - [Speedometer Hand](https://www.gtagaming.com/audi-s5-speedometer-f26088.html)

We hope you enjoy playing "Sandro Run"! If you have any questions or feedback, feel free to open an issue or submit a pull request. Happy gaming! 🎮