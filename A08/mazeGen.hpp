using namespace std;

void Assignment08::createMazeMesh(int row, int col, char **maze) {

    int numVertices = 0;
    int k;

    int shift = 0;

    // 1. create floor

    int cachedVertices[row + 1][col + 1][2];

    for (int i = 0; i < row + 1; ++i) {
        for (int j = 0; j < col + 1; ++j) {
            cachedVertices[i][j][0] = -1;
            cachedVertices[i][j][1] = -1;
        }
    }

    vPos.push_back(0.0f);
    vPos.push_back(0.0f - shift);
    vPos.push_back(0.0f);
    cachedVertices[0][0][0] = numVertices;
    numVertices++;

    vPos.push_back((float) col);
    vPos.push_back(0.0f - shift);
    vPos.push_back(0.0f);
    cachedVertices[0][col][0] = numVertices;
    numVertices++;

    vPos.push_back((float) col);
    vPos.push_back(0.0f - shift);
    vPos.push_back((float) row);
    cachedVertices[row][col][0] = numVertices;
    numVertices++;

    vPos.push_back(0.0f);
    vPos.push_back(0.0f - shift);
    vPos.push_back((float) row);
    cachedVertices[row][0][0] = numVertices;
    numVertices++;

    vIdx.push_back(cachedVertices[0][0][0]);
    vIdx.push_back(cachedVertices[0][col][0]);
    vIdx.push_back(cachedVertices[row][col][0]);

    vIdx.push_back(cachedVertices[0][0][0]);
    vIdx.push_back(cachedVertices[row][col][0]);
    vIdx.push_back(cachedVertices[row][0][0]);

    // 2.1. create wall in z direction
    for (int j = 0; j < col; j++) {
        for (int i = 0; i < row - 1; i++) {
            if (maze[i][j] == '#' && maze[i + 1][j] == '#') {

                if (cachedVertices[i][j][0] == -1) { // 8
                    vPos.push_back((float) j);
                    vPos.push_back(0.0f - shift);
                    vPos.push_back((float) i);
                    cachedVertices[i][j][0] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[i][j][1] == -1) { // 7
                    vPos.push_back((float) j);
                    vPos.push_back(1.0f - shift);
                    vPos.push_back((float) i);
                    cachedVertices[i][j][1] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[i][j + 1][0] == -1) { // 6
                    vPos.push_back((float) j + 1);
                    vPos.push_back(0.0f - shift);
                    vPos.push_back((float) i);
                    cachedVertices[i][j + 1][0] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[i][j + 1][1] == -1) { // 5
                    vPos.push_back((float) j + 1);
                    vPos.push_back(1.0f - shift);
                    vPos.push_back((float) i);
                    cachedVertices[i][j + 1][1] = numVertices;
                    numVertices++;
                }

                k = i + 1;

                while (k + 1 < row && maze[k + 1][j] == '#') {
                    k++;
                }

                if (cachedVertices[k + 1][j][0] == -1) { // 4
                    vPos.push_back((float) j);
                    vPos.push_back(0.0f - shift);
                    vPos.push_back((float) k + 1);
                    cachedVertices[k + 1][j][0] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[k + 1][j][1] == -1) { // 3
                    vPos.push_back((float) j);
                    vPos.push_back(1.0f - shift);
                    vPos.push_back((float) k + 1);
                    cachedVertices[k + 1][j][1] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[k + 1][j + 1][0] == -1) { // 2
                    vPos.push_back((float) j + 1);
                    vPos.push_back(0.0f - shift);
                    vPos.push_back((float) k + 1);
                    cachedVertices[k + 1][j + 1][0] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[k + 1][j + 1][1] == -1) { // 1
                    vPos.push_back((float) j + 1);
                    vPos.push_back(1.0f - shift);
                    vPos.push_back((float) k + 1);
                    cachedVertices[k + 1][j + 1][1] = numVertices;
                    numVertices++;
                }

                vIdx.push_back(cachedVertices[i][j][0]);
                vIdx.push_back(cachedVertices[i][j][1]);
                vIdx.push_back(cachedVertices[k + 1][j][0]);

                vIdx.push_back(cachedVertices[i][j][1]);
                vIdx.push_back(cachedVertices[k + 1][j][1]);
                vIdx.push_back(cachedVertices[k + 1][j][0]);

                vIdx.push_back(cachedVertices[i][j + 1][0]);
                vIdx.push_back(cachedVertices[i][j + 1][1]);
                vIdx.push_back(cachedVertices[k + 1][j + 1][0]);

                vIdx.push_back(cachedVertices[i][j + 1][1]);
                vIdx.push_back(cachedVertices[k + 1][j + 1][1]);
                vIdx.push_back(cachedVertices[k + 1][j + 1][0]);

                if (!((j + 1 < col && maze[i][j + 1] == '#') || (j - 1 >= 0 && maze[i][j - 1] == '#'))) {
                    vIdx.push_back(cachedVertices[i][j][0]);
                    vIdx.push_back(cachedVertices[i][j][1]);
                    vIdx.push_back(cachedVertices[i][j + 1][0]);

                    vIdx.push_back(cachedVertices[i][j][1]);
                    vIdx.push_back(cachedVertices[i][j + 1][0]);
                    vIdx.push_back(cachedVertices[i][j + 1][1]);
                }

                if (!((j + 1 < col && maze[k][j + 1] == '#') || (j - 1 >= 0 && maze[k][j - 1] == '#'))) {
                    vIdx.push_back(cachedVertices[k + 1][j][0]);
                    vIdx.push_back(cachedVertices[k + 1][j][1]);
                    vIdx.push_back(cachedVertices[k + 1][j + 1][0]);

                    vIdx.push_back(cachedVertices[k + 1][j][1]);
                    vIdx.push_back(cachedVertices[k + 1][j + 1][0]);
                    vIdx.push_back(cachedVertices[k + 1][j + 1][1]);
                }

                vIdx.push_back(cachedVertices[i][j][1]);
                vIdx.push_back(cachedVertices[i][j + 1][1]);
                vIdx.push_back(cachedVertices[k + 1][j][1]);

                vIdx.push_back(cachedVertices[i][j + 1][1]);
                vIdx.push_back(cachedVertices[k + 1][j][1]);
                vIdx.push_back(cachedVertices[k + 1][j + 1][1]);

                i = k;
            }
        }
    }

    // 2.2. create wall in x direction
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col - 1; j++) {
            if (maze[i][j] == '#' && maze[i][j + 1] == '#') {

                if (cachedVertices[i][j][0] == -1) { // 8
                    vPos.push_back((float) j);
                    vPos.push_back(0.0f - shift);
                    vPos.push_back((float) i);
                    cachedVertices[i][j][0] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[i][j][1] == -1) { // 7
                    vPos.push_back((float) j);
                    vPos.push_back(1.0f - shift);
                    vPos.push_back((float) i);
                    cachedVertices[i][j][1] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[i + 1][j][0] == -1) { // 6
                    vPos.push_back((float) j);
                    vPos.push_back(0.0f - shift);
                    vPos.push_back((float) i + 1);
                    cachedVertices[i + 1][j][0] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[i + 1][j][1] == -1) { // 5
                    vPos.push_back((float) j);
                    vPos.push_back(1.0f - shift);
                    vPos.push_back((float) i + 1);
                    cachedVertices[i + 1][j][1] = numVertices;
                    numVertices++;
                }

                k = j + 1;

                while (k + 1 < col && maze[i][k + 1] == '#') {
                    k++;
                }

                if (cachedVertices[i][k + 1][0] == -1) { // 4
                    vPos.push_back((float) k + 1);
                    vPos.push_back(0.0f - shift);
                    vPos.push_back((float) i);
                    cachedVertices[i][k + 1][0] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[i][k + 1][1] == -1) { // 3
                    vPos.push_back((float) k + 1);
                    vPos.push_back(1.0f - shift);
                    vPos.push_back((float) i);
                    cachedVertices[i][k + 1][1] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[i + 1][k + 1][0] == -1) { // 2
                    vPos.push_back((float) k + 1);
                    vPos.push_back(0.0f - shift);
                    vPos.push_back((float) i + 1);
                    cachedVertices[i + 1][k + 1][0] = numVertices;
                    numVertices++;
                }

                if (cachedVertices[i + 1][k + 1][1] == -1) { // 1
                    vPos.push_back((float) k + 1);
                    vPos.push_back(1.0f - shift);
                    vPos.push_back((float) i + 1);
                    cachedVertices[i + 1][k + 1][1] = numVertices;
                    numVertices++;
                }

                vIdx.push_back(cachedVertices[i][j][0]);
                vIdx.push_back(cachedVertices[i][j][1]);
                vIdx.push_back(cachedVertices[i][k + 1][0]);

                vIdx.push_back(cachedVertices[i][j][1]);
                vIdx.push_back(cachedVertices[i][k + 1][1]);
                vIdx.push_back(cachedVertices[i][k + 1][0]);

                vIdx.push_back(cachedVertices[i + 1][j][0]);
                vIdx.push_back(cachedVertices[i + 1][j][1]);
                vIdx.push_back(cachedVertices[i + 1][k + 1][0]);

                vIdx.push_back(cachedVertices[i + 1][j][1]);
                vIdx.push_back(cachedVertices[i + 1][k + 1][1]);
                vIdx.push_back(cachedVertices[i + 1][k + 1][0]);

                if (!((i + 1 < row && maze[i + 1][j] == '#') || (i - 1 >= 0 && maze[i - 1][j] == '#'))) {
                    vIdx.push_back(cachedVertices[i][j][0]);
                    vIdx.push_back(cachedVertices[i][j][1]);
                    vIdx.push_back(cachedVertices[i + 1][j][0]);

                    vIdx.push_back(cachedVertices[i][j][1]);
                    vIdx.push_back(cachedVertices[i + 1][j][0]);
                    vIdx.push_back(cachedVertices[i + 1][j][1]);
                }

                if (!((i + 1 < row && maze[i + 1][k] == '#') || (i - 1 >= 0 && maze[i - 1][k] == '#'))) {
                    vIdx.push_back(cachedVertices[i][k + 1][0]);
                    vIdx.push_back(cachedVertices[i][k + 1][1]);
                    vIdx.push_back(cachedVertices[i + 1][k + 1][0]);

                    vIdx.push_back(cachedVertices[i][k + 1][1]);
                    vIdx.push_back(cachedVertices[i + 1][k + 1][0]);
                    vIdx.push_back(cachedVertices[i + 1][k + 1][1]);
                }

                vIdx.push_back(cachedVertices[i][j][1]);
                vIdx.push_back(cachedVertices[i + 1][j][1]);
                vIdx.push_back(cachedVertices[i][k + 1][1]);

                vIdx.push_back(cachedVertices[i + 1][j][1]);
                vIdx.push_back(cachedVertices[i][k + 1][1]);
                vIdx.push_back(cachedVertices[i + 1][k + 1][1]);

                j = k;
            }
        }
    }
}