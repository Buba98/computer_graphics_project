using namespace std;

void createVertex(std::vector<float> &vPos, int row, int col, int h,
                  int &numVertices,
                  std::vector<std::vector<std::vector<int>>> &cachedVertices) {
    if (cachedVertices[row][col][h] == -1) {
        vPos.push_back((float)col);
        vPos.push_back((float)h);
        vPos.push_back((float)row);
        cachedVertices[row][col][h] = numVertices;
        numVertices++;
    }
}

void Assignment08::createMazeMesh(int row, int col, char **maze) {
    int numVertices = 0;
    int k;

    std::vector<std::vector<std::vector<int>>> cachedVertices;

    for (int i = 0; i < row + 1; ++i) {
        cachedVertices.push_back(std::vector<std::vector<int>>());
        for (int j = 0; j < col + 1; ++j) {
            cachedVertices[i].push_back(std::vector<int>());
            for (int k = 0; k < 2; ++k) {
                cachedVertices[i][j].push_back(-1);
            }
        }
    }

    // 1. create floor

    createVertex(vPos, 0, 0, 0, numVertices, cachedVertices);

    createVertex(vPos, 0, col, 0, numVertices, cachedVertices);

    createVertex(vPos, row, col, 0, numVertices, cachedVertices);

    createVertex(vPos, row, 0, 0, numVertices, cachedVertices);

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
                createVertex(vPos, i, j, 0, numVertices, cachedVertices);

                createVertex(vPos, i, j, 1, numVertices, cachedVertices);

                createVertex(vPos, i, j + 1, 0, numVertices, cachedVertices);

                createVertex(vPos, i, j + 1, 1, numVertices, cachedVertices);

                k = i + 1;

                while (k + 1 < row && maze[k + 1][j] == '#') {
                    k++;
                }

                createVertex(vPos, k + 1, j, 0, numVertices, cachedVertices);

                createVertex(vPos, k + 1, j, 1, numVertices, cachedVertices);

                createVertex(vPos, k + 1, j + 1, 0, numVertices,
                             cachedVertices);

                createVertex(vPos, k + 1, j + 1, 1, numVertices,
                             cachedVertices);

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

                if (!((j + 1 < col && maze[i][j + 1] == '#') ||
                      (j - 1 >= 0 && maze[i][j - 1] == '#'))) {
                    vIdx.push_back(cachedVertices[i][j][0]);
                    vIdx.push_back(cachedVertices[i][j][1]);
                    vIdx.push_back(cachedVertices[i][j + 1][0]);

                    vIdx.push_back(cachedVertices[i][j][1]);
                    vIdx.push_back(cachedVertices[i][j + 1][0]);
                    vIdx.push_back(cachedVertices[i][j + 1][1]);
                }

                if (!((j + 1 < col && maze[k][j + 1] == '#') ||
                      (j - 1 >= 0 && maze[k][j - 1] == '#'))) {
                    vIdx.push_back(cachedVertices[k + 1][j][0]);
                    vIdx.push_back(cachedVertices[k + 1][j][1]);
                    vIdx.push_back(cachedVertices[k + 1][j + 1][0]);

                    vIdx.push_back(cachedVertices[k + 1][j][1]);
                    vIdx.push_back(cachedVertices[k + 1][j + 1][0]);
                    vIdx.push_back(cachedVertices[k + 1][j + 1][1]);
                }

                // Roof
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
                createVertex(vPos, i, j, 0, numVertices, cachedVertices);

                createVertex(vPos, i, j, 1, numVertices, cachedVertices);

                createVertex(vPos, i + 1, j, 0, numVertices, cachedVertices);

                createVertex(vPos, i + 1, j, 1, numVertices, cachedVertices);

                k = j + 1;

                while (k + 1 < col && maze[i][k + 1] == '#') {
                    k++;
                }

                createVertex(vPos, i, k + 1, 0, numVertices, cachedVertices);

                createVertex(vPos, i, k + 1, 1, numVertices, cachedVertices);

                createVertex(vPos, i + 1, k + 1, 0, numVertices,
                             cachedVertices);

                createVertex(vPos, i + 1, k + 1, 1, numVertices,
                             cachedVertices);

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

                if (!((i + 1 < row && maze[i + 1][j] == '#') ||
                      (i - 1 >= 0 && maze[i - 1][j] == '#'))) {
                    vIdx.push_back(cachedVertices[i][j][0]);
                    vIdx.push_back(cachedVertices[i][j][1]);
                    vIdx.push_back(cachedVertices[i + 1][j][0]);

                    vIdx.push_back(cachedVertices[i][j][1]);
                    vIdx.push_back(cachedVertices[i + 1][j][0]);
                    vIdx.push_back(cachedVertices[i + 1][j][1]);
                }

                if (!((i + 1 < row && maze[i + 1][k] == '#') ||
                      (i - 1 >= 0 && maze[i - 1][k] == '#'))) {
                    vIdx.push_back(cachedVertices[i][k + 1][0]);
                    vIdx.push_back(cachedVertices[i][k + 1][1]);
                    vIdx.push_back(cachedVertices[i + 1][k + 1][0]);

                    vIdx.push_back(cachedVertices[i][k + 1][1]);
                    vIdx.push_back(cachedVertices[i + 1][k + 1][0]);
                    vIdx.push_back(cachedVertices[i + 1][k + 1][1]);
                }

                // Roof
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