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
#define DAY 0
#define SUNSET 1
#define NIGHT 2
#define GUARD_RAIL_WIDTH 0.17f
#define ONE_POINT_SCORE_DISTANCE 100

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
#define STARTING_YAW 0.0f
#define STARTING_PITCH (M_PI / 2.5f)

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
#define MOTO_LENGTH 2.2f
#define MOTO_WIDTH 0.6f
#define MOTO_HEIGHT 1.2f
#define MOTO_MODEL_OFFSET 0.3f

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
#define CENTER_LEFT_LANE_CAR_SPEED (-16)
#define LEFT_LANE_CAR_SPEED (-12)
#define AVOID_INITIAL_OVERLAP_OFFSET 15
#define INITIAL_RIGHT_LANES_SHIFTING_FACTOR 0.6f

const float carsLength[10] = {3.9f, 5.1f, 4.5f, 4.6f, 5.0f, 4.5f, 4.8f, 8.25f, 8.5f, 10.1f};
const float carsWidth[10] = {1.8f, 1.85f, 1.81f, 1.88f, 1.78f, 1.9f, 1.75f, 2.4f, 2.35f, 2.14f};

#endif