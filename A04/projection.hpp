
#include <glm/gtx/transform2.hpp>

void SetupProjectionMatrices(Assignment04 *A, float Ar)
{
	/**************
	 Method Matrix(int N, glm::mat4 M) of object A,
	 receives in N the number of the matrix that needs to be set, and the matrix in M
	***************/

	glm::mat4 S;
	float w = 10.0f;
	float n = -50.0f;
	float f = 50.0f;
	glm::mat4 P = glm::scale(glm::mat4(1), glm::vec3(1, -1, 1)) * glm::ortho(-w, w, -w / Ar, w / Ar, n, f);

	// Isometric projection
	S = P * glm::rotate(glm::mat4(1), glm::radians(35.26f), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(0, 1, 0));
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Dimetric
	S = P * glm::rotate(glm::mat4(1), glm::radians(20.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(0, 1, 0));
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Trimetric
	S = P * glm::rotate(glm::mat4(1), glm::radians(20.0f), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1), glm::radians(30.0f), glm::vec3(0, 1, 0));
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Cabinet
	S = P * shearZ3D(glm::mat4(1), -0.5f * glm::cos(45.0f), -0.5f * glm::cos(45.0f));
	A->Matrix(4, S); // sets the matrix corresponding to piece 4
}
