

void SetupProjectionMatrices(Assignment05 *A, float Ar)
{
	/**************
	 Method Matrix(int N, glm::mat4 M) of object A,
	 receives in N the number of the matrix that needs to be set, and the matrix in M
	***************/

	glm::mat4 S;
	glm::mat4 P;
	float n = 0.1f;
	float f = 50.0f;

	// Fov-y = 60 deg
	S = glm::perspective(glm::radians(60.0f), Ar, n, f);
	S[1][1] *= -1;
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Fov-y = 105 deg
	S = glm::perspective(glm::radians(105.0f), Ar, n, f);
	S[1][1] *= -1;
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Fov-y = 25 deg
	S = glm::perspective(glm::radians(25.0f), Ar, n, f);
	S[1][1] *= -1;
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Right View, Fov-x = 45 deg

	float teta = glm::radians(45.0f);
	float t = n  * glm::tan(teta / 2);
	float b = -t;
	float r = Ar * t;
	float l = -r;

	S = glm::scale(glm::mat4(1), glm::vec3(1, -1, 1)) * glm::frustum(0.0f, 2.0f * r , b, t, n, f);
	A->Matrix(4, S); // sets the matrix corresponding to piece 4
}
