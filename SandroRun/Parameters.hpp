#ifndef PARAMETERS
#define PARAMETERS

// World settings
#define TERRAIN_LENGTH 120
#define ROAD_WIDTH 20.0f
#define TERRAIN_WIDTH 30.0f
#define NUM_TERRAIN_REPETITIONS 4
#define NUM_TREE_PER_LINE (2 * NUM_TERRAIN_REPETITIONS)
#define NUM_RAIL_PER_LINE (NUM_TERRAIN_REPETITIONS)
#define WORLD_LENGTH (2 * TERRAIN_LENGTH)
#define NUM_LIGHTS_PER_LINE (2 * NUM_TERRAIN_REPETITIONS)

// Camera settings
#define FOV_Y glm::radians(60.0f)
#define NEAR_PLANE 0.1f
#define FAR_PLANE 200.0f
#define CAM_HEIGHT 1.0f
#define CAM_DIST 5.0f
#define MIN_PITCH glm::radians(10.0f)
#define MAX_PITCH glm::radians(80.0f)
#define MAX_YAW ((float) M_PI / 4.0f)
#define MIN_YAW (-MAX_YAW)
#define ROT_SPEED glm::radians(120.0f)

// Moto settings
#define Z_SPEED 3.0f
#define X_SPEED 21.0f
#define MIN_MOTO_ROLL (float) (-M_PI / 4.0f)
#define MAX_MOTO_ROLL (-MIN_MOTO_ROLL)
#define MIN_MOTO_PITCH 0.0f
#define MAX_MOTO_PITCH (float) (M_PI / 4.0f)
#define MOTO_ROLL_SPEED 100
#define MOTO_PITCH_SPEED 150
#define LAMBDA 10.0f

// Cars settings
#define NUM_CAR_MODELS 10
#define NUM_CAR_MODEL_INSTANCES 1
#define NUM_CAR_PALETTES 5
#define RIGHT_LANE 6.5f
#define CENTER_RIGHT_LANE 2.2f
#define CENTER_LEFT_LANE (-CENTER_RIGHT_LANE)
#define LEFT_LANE (-RIGHT_LANE)
#define RIGHT_LANE_CAR_SPEED 18
#define CENTER_RIGHT_LANE_CAR_SPEED 25
#define CENTER_LEFT_LANE_CAR_SPEED -16
#define LEFT_LANE_CAR_SPEED -12
#define AVOID_INITIAL_OVERLAP_OFFSET 15

enum TimeOfDay {
    DAY = 0,
    SUNSET = 1,
    NIGHT = 2
};

#endif