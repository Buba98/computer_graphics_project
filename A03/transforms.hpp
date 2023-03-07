#define GLM_FORCE_RADIANS
#include <glm/gtx/transform2.hpp>

void SetupMatrices(Assignment03 *A)
{
	/**************
	 Method Matrix(int N, glm::mat4 M) of object A,
	 receives in N the number of the matrix that needs to be set, and the matrix in M
	***************/

	//  glm::shearY3D(glm::mat4(1), 2.0f, 2.0f)

	glm::mat4 S;
	glm::mat4 trans;
	glm::mat4 rot;
	glm::mat4 scale;

	// To solve the puzzle: scale 2x proportionally, centered in (2,0,-3)

	trans = glm::translate(glm::mat4(1), glm::vec3(2, 0, -3));
	scale = glm::scale(glm::mat4(1), glm::vec3(2, 2, 2));

	S = trans * 
		scale * 
		glm::inverse(trans);
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// To solve the puzzle: scale 2x on an axis oriented 45 degrees between the x and z axis and passing through point (1,0,0)
	
	trans = glm::translate(glm::mat4(1), glm::vec3(1, 0, 0));
	rot = glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(0, 1, 0));
	scale = glm::scale(glm::mat4(1), glm::vec3(3, 1, 1));

	S =
		trans *
		rot *
		scale *
		glm::inverse(rot) *
		glm::inverse(trans);
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// To solve the puzzle: find it yourself! Hint: compose a rotation around an arbitrary point with a translation
	
	trans = glm::translate(glm::mat4(1), glm::vec3(2, 0, 3));
	rot = glm::rotate(glm::mat4(1), glm::radians(-45.0f), glm::vec3(0, 1, 0));
	
	S = glm::translate(glm::mat4(1), glm::vec3(1, 0, 2)) *
		glm::inverse(rot) *
		glm::inverse(trans);
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// To solve the puzzle: rotate 60 degree around the y axis, centered in (-1,0,-2)
	
	trans = glm::translate(glm::mat4(1), glm::vec3(-1, 0, -2));
	rot = glm::rotate(glm::mat4(1), glm::radians(60.0f), glm::vec3(0, 1, 0));
	
	S = trans *
		rot *
		glm::inverse(trans);
	A->Matrix(4, S); // sets the matrix corresponding to piece 4

	// To solve the puzzle: rotate -90 degree around an arbitrary axis passing in (-1,0,0). The x axis can be aligned to this arbitrary direction with a rotation of -45 around the y axis.
	
	trans = glm::translate(glm::mat4(1), glm::vec3(-1, 0, 0));
	rot = glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(0, 1, 0));
	
	S = trans *
		rot *
		glm::inverse(glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 0, 1))) *
		glm::inverse(rot) *
		glm::inverse(trans);
	A->Matrix(5, S); // sets the matrix corresponding to piece 5

	// To solve the puzzle: find it yourself! Hint: compose a non-proportional scaling that halves the object along one of the main axes, centered in an arbitrary point, with a translation
	trans = glm::translate(glm::mat4(1), glm::vec3(-3, 0, 2));
	scale = glm::scale(glm::mat4(1), glm::vec3(2, 1, 1));
	
	S = glm::translate(glm::mat4(1), glm::vec3(-1, 0, 2)) * 
		glm::inverse(scale) * 
		glm::inverse(trans);
	A->Matrix(6, S); // sets the matrix corresponding to piece 6
}
