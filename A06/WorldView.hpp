
glm::mat4 MakeViewProjectionMatrix(float Ar, float Alpha, float Beta, float Rho, glm::vec3 Pos) {
    // Creates a view projection matrix, with near plane at 0.1, and far plane at 50.0, and
    // aspect ratio given in <Ar>. The view matrix, uses the Look-in-Direction model, with
    // vector <pos> specifying the position of the camera, and angles <Alpha>, <Beta> and <Rho>
    // defining its direction. In particular, <Alpha> defines the direction (Yaw), <Beta> the
    // elevation (Pitch), and <Rho> the roll.

    const float near = 0.1f;
    const float far = 50.0f;
    const float fov_y = glm::radians(45.0f);

    glm::mat4 Proj = glm::perspective(fov_y, Ar, near, far);
    Proj[1][1] *= -1;

    glm::mat4 View =
            glm::rotate(glm::mat4(1.0), -Rho, glm::vec3(0, 0, 1)) *
            glm::rotate(glm::mat4(1.0), -Beta, glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(1.0), -Alpha, glm::vec3(0, 1, 0)) *
            glm::translate(glm::mat4(1.0), -Pos);

    glm::mat4 ViewProj = Proj * View;

    return ViewProj;
}

glm::mat4 MakeWorldMatrix(glm::vec3 pos, glm::quat rQ, glm::vec3 size) {
    // creates and returns a World Matrix that positions the object at <pos>,
    // orients it according to <rQ>, and scales it according to the sizes
    // given in vector <size>

    const glm::mat4 Trans = glm::translate(glm::mat4(1), pos);
    const glm::mat4 Rot = glm::mat4(rQ);
    const glm::mat4 Scale = glm::scale(glm::mat4(1), size);

    glm::mat4 World = Trans * Rot * Scale;

    return World;
}
