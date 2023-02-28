#define MAX_BYTE 255.0

void SetupTriangles(Assignment01 *A)
{
	/**************
	 Function Triangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b)

	 draws a triangle connecting points (x1,y1)  (x2, y2) and (x3, y3), colored with the (r,g,b) specified color.
	 vertex are expressed in Normalized screen coordinates, (i.e. in range [-1,+1]), following the Vulkan convention.
	 The red, green and blue (r,g,b) values are in the range [0,1].

	 Using function Triangle(...) try to draw a children house, similar to the one below.

	 First remove the two function below (given only as an example), and replace them with your own.
	 Since it is a C program, you can use for loops if you think it can be helpful in your drawing.
	 The for() command below that draws a sun like shape in exact center of the screen near the top,
	 is an example on how you can use it: please remove it, and replace it with your own code.
	 It is not necessery to use a for loop if you do not really need it: it has been put here only as
	 an example to help students not familiar with C++.
	***************/

	A->Triangle(-0.25, 0.1, 0.1, -0.4, 0.1, 0.6, 1, 0, 0);											  // Draws the body red triangole
	A->Triangle(0.1, 0.3, 0.1, -0.4, 0.4, 0, 0, 1, 0);												  // Draws the body green triangle
	A->Triangle(0.1, -0.4, 0.3, -0.15, 0.3, -0.65, 171 / MAX_BYTE, 85 / MAX_BYTE, 186 / MAX_BYTE);	  // Draws the head purple triangle
	A->Triangle(0.5, -0.4, 0.3, -0.15, 0.3, -0.65, 171 / MAX_BYTE, 85 / MAX_BYTE, 186 / MAX_BYTE);	  // Draws the head purple triangle
	A->Triangle(0.3, -0.65, 0.5, -0.4, 0.5, -0.9, 129 / MAX_BYTE, 211 / MAX_BYTE, 225 / MAX_BYTE);	  // Draws the right ear triangle
	A->Triangle(0.3, -0.65, 0.1, -0.4, 0.1, -0.9, 250 / MAX_BYTE, 168 / MAX_BYTE, 190 / MAX_BYTE);	  // Draws the left ear triangle
	A->Triangle(-0.25, 0.1, 0.25, 0.85, -0.25, 0.85, 69 / MAX_BYTE, 175 / MAX_BYTE, 225 / MAX_BYTE);  // Draws the back triangle
	A->Triangle(-0.25, 0.85, -0.45, 0.6, -0.65, 0.85, 173 / MAX_BYTE, 200 / MAX_BYTE, 1 / MAX_BYTE); // Draws the rightmost tail triangle
	A->Triangle(-0.8, 0.6, -0.45, 0.6, -0.65, 0.85, 173 / MAX_BYTE, 200 / MAX_BYTE, 1 / MAX_BYTE);	  // Draws the leftmost tail triangle
}
