#ifndef PARAMETERS
#define PARAMETERS

// World settings
#define TERRAIN_LENGTH 120
#define ROAD_WIDTH 20.0f
#define TERRAIN_WIDTH 30.0f
#define PERIODS 4

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
#define NUM_CAR_MODELS 3
#define NUM_CAR_MODEL_INSTANCES 3
#define RIGHT_LANE 6.5f
#define CENTER_RIGHT_LANE 2.2f
#define CENTER_LEFT_LANE (-CENTER_RIGHT_LANE)
#define LEFT_LANE (-RIGHT_LANE)
#define MAX_CAR_SPEED 35
#define MIN_CAR_SPEED 15
#define FRONT_COLLISION_Z_COORD_OFFSET (7 * TERRAIN_LENGTH)
#define BACK_COLLISION_Z_COORD_OFFSET (2 * TERRAIN_LENGTH)
#define AVOID_INITIAL_OVERLAP_OFFSET 30

#define NUM_TREE_PER_LINE (2 * PERIODS)
#define NUM_RAIL_PER_LINE (PERIODS)

#endif