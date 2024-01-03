# Sandro run
This is the repository for the Computer Graphics course project from Politecnico di Milano.
The project is a 3D game developed in C++ using Vulkan APIs.
The project features:
- Splash screens
- The motorbike of the main character
- 3 sky-boxes based on the time of the day
- A periodic scenery composed by:
  - A terrain with hills
  - Trees
  - A Street
  - Guard rails
  - Streetlights
- Cars that move on the street in both directions
- Sounds for the motorbike
- Two camera that follows the motorbike
  - A third person camera
  - A first person camera
- Collision detection between the motorbike and the cars
- Collision detection between the motorbike and the guard rails
- Score system
- Game over screen
- A presentation mode that deactivates the collision detections and the camera limits

## How to run
To run the project you need to have the Vulkan SDK installed on your machine.
To install it, follow the instructions on the [website](https://vulkan-tutorial.com/Development_environment).
In order to build the project, you need to run the following commands:
```
mkdir build
cd build
cmake ../
make
```
In order to run the project, you need to run the following command:
```
cd build/bin
./Sandro_run
```

## Licenses
The project uses the following objects:
- 3D models:
  - [Moto 3D model](https://www.cgtrader.com/free-3d-models/vehicle/motorcycle/suzuki-gsx-750-bike)
  - [Guard rail 3D model](https://www.cgtrader.com/free-3d-models/exterior/street-exterior/guardrail-fd10776d-d363-4be6-b5bd-6853aa50c539)
  - [Tree 3D model](https://www.cgtrader.com/free-3d-models/plant/leaf/low-poly-plant-set)
  - [Car 3D model](https://www.cgtrader.com/free-3d-models/car/car/low-poly-cars-03d6cf0c-0882-44e2-a22c-02d4f8e68eeb)
- Textures:
  - [Road texture](https://www.sketchuptextureclub.com/textures/architecture/roads/roads/cracks-road-pbr-texture-seamless-21574)