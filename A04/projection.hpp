#include <glm/gtx/transform2.hpp>

void SetupProjectionMatrices(Assignment04 *A, float Ar) {
    /**************
     Method Matrix(int N, glm::mat4 M) of object A,
     receives in N the number of the matrix that needs to be set, and the matrix in M
    ***************/

    glm::mat4 S;
    glm::mat4 P_proj;
    glm::mat4 Ry;
    glm::mat4 Rx;

    float half_width = 10.0;
    float near_plane = -50.0;
    float far_plane = 50.0;
	float top = half_width / Ar;
	float bottom = -top;

    float alpha, beta;

    // matrix built specifing each column vector
    P_proj = glm::scale(glm::mat4(1), glm::vec3(1, -1, 1)) *
                       glm::ortho(-half_width, half_width, bottom, top, near_plane, far_plane);

    // Isometric projection
    alpha = glm::radians(35.26f);
    beta = glm::radians(-45.0f);
    Rx = glm::rotate(glm::mat4(1), alpha, glm::vec3(1, 0, 0));
    Ry = glm::rotate(glm::mat4(1), beta, glm::vec3(0, 1, 0));
    S = P_proj * Rx * Ry;
    A->Matrix(1, S);  // sets the matrix corresponding to piece 1

    // Dimetric
    alpha = glm::radians(20.0f);
    beta = glm::radians(-45.0f);
    Rx = glm::rotate(glm::mat4(1), alpha, glm::vec3(1, 0, 0));
    Ry = glm::rotate(glm::mat4(1), beta, glm::vec3(0, 1, 0));
    S = P_proj * Rx * Ry;
    A->Matrix(2, S);  // sets the matrix corresponding to piece 2

    // Trimetric
    alpha = glm::radians(30.0f);
    beta = glm::radians(-70.0f);
    Rx = glm::rotate(glm::mat4(1), alpha, glm::vec3(1, 0, 0));
    Ry = glm::rotate(glm::mat4(1), beta, glm::vec3(0, 1, 0));
    S = P_proj * Rx * Ry;
    A->Matrix(3, S);  // sets the matrix corresponding to piece 3

    // Cabinet
    alpha = glm::radians(45.0f);
    float p = 0.5;  // ro
    float hx = (-1) * p * cos(alpha);
    float hy = (-1) * p * sin(alpha);
    S = glm::shearZ3D(glm::mat4(1), hx, hy);
    S = P_proj * S;
    A->Matrix(4, S);  // sets the matrix corresponding to piece 4
}
