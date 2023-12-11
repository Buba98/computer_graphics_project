#include "../Starter.hpp"
#include "Road.hpp"

#define FRONT 10
#define BACK 1

///
/// \param position position of the character
///
/// \brief Draws the road in front for FRONT units and behind for BACK units
///
void SandroRun::drawRoad(std::vector<VertexMesh> &vDef, std::vector<uint32_t> &vIdx, double position) {

    int pos = (int) position - BACK;

    for (int i = 0; i < BACK + FRONT; i++) {

        vDef.push_back({{-0.5f, 0.0f, (float) pos},
                        {0.0f,  1.0f, 0.0f},
                        {0,     0}});

        vDef.push_back({{0.5f, 0.0f, (float) pos},
                        {0.0f,  1.0f, 0.0f},
                        {1,     0}});

        vDef.push_back({{-0.5f, 0.0f, (float) pos + 1},
                        {0.0f,  1.0f, 0.0f},
                        {0,     1}});

        vDef.push_back({{0.5f, 0.0f, (float) pos + 1},
                        {0.0f,  1.0f, 0.0f},
                        {1,     1}});
    }

    for (int i = 0; i < BACK + FRONT; i++) {
        vIdx.push_back(4 * i);
        vIdx.push_back(4 * i + 1);
        vIdx.push_back(4 * i + 2);

        vIdx.push_back(4 * i + 3);
        vIdx.push_back(4 * i + 2);
        vIdx.push_back(4 * i + 1);
    }

}

