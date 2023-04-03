using namespace std;

void Assignment08::createMazeMesh(int row, int col, char **maze) {

    int walls[row + 1][col + 1];

    for (int i = 0; i < row + 1; i++) {
        for (int j = 0; j < col + 1; j++) {
            walls[i][j] = 0;
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (maze[i][j] == '#') {
                walls[i][j] = !walls[i][j];
                walls[i][j + 1] = !walls[i][j + 1];
                walls[i + 1][j] = !walls[i + 1][j];
                walls[i + 1][j + 1] = !walls[i + 1][j + 1];
            }
        }
    }

    int order = 1;

    for (int i = 0; i < row + 1; i++) {
        for (int j = 0; j < col + 1; j++) {
            if (walls[i][j]) {
                walls[i][j] = order;
                order++;
            }
        }
    }

    //Create the vertices
    for (float i = 0; i < row + 1; i++) {
        for (float j = 0; j < col + 1; j++) {
            if (walls[(int) i][(int) j]) {
                vPos.push_back(i);
                vPos.push_back(0.0);
                vPos.push_back(j);
                vPos.push_back(i);
                vPos.push_back(1.0);
                vPos.push_back(j);
            }
        }
    }

    //horizontal walls
    for (int i = 0; i < (order - 1) * 2 - 3; i = i + 4) {
        vIdx.push_back(i);
        vIdx.push_back(i + 1);
        vIdx.push_back(i + 2);
        vIdx.push_back(i + 1);
        vIdx.push_back(i + 2);
        vIdx.push_back(i + 3);
    }

    //Create the floor
    vIdx.push_back((walls[0][0] - 1) * 2);
    vIdx.push_back((walls[0][col] - 1) * 2);
    vIdx.push_back((walls[row][0] - 1) * 2);

    vIdx.push_back((walls[0][col] - 1) * 2);
    vIdx.push_back((walls[row][0] - 1) * 2);
    vIdx.push_back((walls[row][col] - 1) * 2);


    int first;
    //vertical walls
    for (int i = 0; i < col + 1; i++) {
        first = 0;
        for (int j = 0; j < row + 1; j++) {
            if (walls[j][i]) {
                if (first) {
                    //wall
                    vIdx.push_back((first - 1) * 2);
                    vIdx.push_back((first - 1) * 2 + 1);
                    vIdx.push_back((walls[j][i] - 1) * 2);

                    vIdx.push_back((first - 1) * 2 + 1);
                    vIdx.push_back((walls[j][i] - 1) * 2);
                    vIdx.push_back((walls[j][i] - 1) * 2 + 1);

                    first = 0;
                } else {
                    first = walls[j][i];
                }
            }
        }
    }

    //Create horizontal ceilings
    for (int i = 0; i < (order - 1) * 2 - 3; i = i + 4) {

        if ((int) vPos[i * 3] % 2 == 1) {
            continue;
        }

        vIdx.push_back(i + 1);
        vIdx.push_back(i + 3);
        vPos.push_back(vPos[(i + 1) * 3] + 1.0);
        vPos.push_back(1.0);
        vPos.push_back(vPos[(i + 1) * 3 + 2]);
        vIdx.push_back(vPos.size() / 3 - 1);


        vIdx.push_back(vPos.size() / 3 - 1);
        vIdx.push_back(i + 3);
        vPos.push_back(vPos[(i + 3) * 3] + 1.0);
        vPos.push_back(1.0);
        vPos.push_back(vPos[(i + 3) * 3 + 2]);
        vIdx.push_back(vPos.size() / 3 - 1);
    }

    //Create vertical ceilings
//    for (int i = 0; i < col + 1; i++) {
//        first = 0;
//        for (int j = 0; j < row + 1; j++) {
//            if (walls[j][i]) {
//                if (first) {
//                    vIdx.push_back((first - 1) * 2 + 1);
//                    vIdx.push_back((walls[j][i] - 1) * 2 + 1);
//                    vPos.push_back(vPos[(first - 1) * 6]);
//                    vPos.push_back(1.0);
//                    vPos.push_back(vPos[(first - 1) * 6 + 2] + 1.0);
//                    first = 0;
//                } else {
//                    first = walls[j][i];
//                }
//            }
//        }
//    }

}
