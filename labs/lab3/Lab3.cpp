#include "Lab3.h"
#include "GLFWApplication/GLFWApplication.h"
#include "GeometricTools/GeometricTools.h"
#include "Rendering/RenderCommands.h"
#include "GLFWApplication/errorHandling.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Shaders/Shader.h"


#include <iostream>
#include <memory>


using namespace GeometricTools;
using namespace RenderCommands;

glm::vec3 select_tile_key(GLFWwindow *window, glm::vec2 selectedTile, bool isPressed, glm::vec2 gridSize);
glm::vec2 rotate_cube_key(GLFWwindow *window, glm::vec2 rotation);
glm::vec3 scale(float scaleFactor);
glm::mat4 transformationMatrix(glm::vec2 rotation);


int lab3_App::run() {


    // Enabling capture of debug output to get messages about potential issues.
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // Print OpenGL context information.
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";



    auto lab_name = "labs";

    auto cubeShaderProgram = std::make_shared<Shader>(lab_name, "plaincube");
    auto outlineShaderProgram = std::make_shared<Shader>(lab_name, "outline");


    //////////////// Cube //////////////////

    // The layout of the cube
    auto cube = UnitCube;


    // The cube indices
    std::array<GLuint, 6*6> cube_indices = UnitCubeTopologyTriangles;

    // Create buffers and arrays
    auto cubeVertexArray = std::make_shared<VertexArray>();


    auto cubeVertexBuffer = std::make_shared<VertexBuffer>(cube.data(), cube.size());

    auto cubeBufferLayout = BufferLayout({{ShaderDataType::Float3, "position"}});

    auto cubeIndexBuffer = std::make_shared<IndexBuffer>(cube_indices.data(), cube_indices.size(), cubeBufferLayout);


    // Connect all the arrays
    cubeVertexBuffer->SetLayout(cubeBufferLayout);
    cubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
    cubeVertexArray->SetIndexBuffer(cubeIndexBuffer);


    ////////////////////////////////////////


    // The default selected tile
    auto selectedTile = glm::vec2(0, 0);

    // The cube's rotation
    glm::vec2 rotation = {0.0f, 0.0f};

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);


    // Application loop code
    while (!glfwWindowShouldClose(window)) {

        // Update cube rotation
        rotation = rotate_cube_key(window, rotation);


        // Poll for events (like input)
        glfwPollEvents();

        // Clear the screen
        Clear();

        // Background color
        SetClearColor(glm::vec4(0.5f, 0.3f, 0.7f, 1.0f));





        // Unit cube outline
        outlineShaderProgram->Bind();
        cubeVertexArray->Bind();

        // Transform the grid into the correct transformation matrix
        outlineShaderProgram->UploadUniformMatrix4fv("u_TransformationMat", transformationMatrix(rotation));

        SetWireframeMode();
        DrawIndex(cubeVertexArray, GL_TRIANGLES);


        // Unit cube
        cubeShaderProgram->Bind();

        // Transform the grid into the correct transformation matrix
        cubeShaderProgram->UploadUniformMatrix4fv("u_TransformationMat", transformationMatrix(rotation));

        SetSolidMode();
        DrawIndex(cubeVertexArray, GL_TRIANGLES);


        // Swap buffers to show our shape
        glfwSwapBuffers(window);



        // The window closes when ESC is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;

    }



    outlineShaderProgram->Unbind();
    cubeShaderProgram->Unbind();

    glfwTerminate();

    return  EXIT_SUCCESS;

}


// rotation.x & rotation.y = how many degrees of rotation per axis
glm::vec2 rotate_cube_key(GLFWwindow *window, glm::vec2 rotation) {

    float rotateSpeed = 0.05f;

    // The keys and their corresponding actions
    std::vector<int> keys = {GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_DOWN, GLFW_KEY_UP};

    std::vector<glm::vec2> actions = {
        {rotation.x - rotateSpeed, rotation.y},
        {rotation.x + rotateSpeed, rotation.y},
        {rotation.x, rotation.y + rotateSpeed},
        {rotation.x, rotation.y - rotateSpeed}
    };

    // Decide between either checking for a new key to be pressed
    // or for an already pressed key to be released


    for (int i = 0; i < keys.size(); ++i) {
        if (glfwGetKey(window, keys[i]) == GLFW_PRESS) { rotation = actions[i];}
    }

    return {rotation};
}


glm::vec3 select_tile_key(GLFWwindow *window, glm::vec2 selectedTile, bool isPressed, glm::vec2 gridSize) {

    glm::vec2 prevTile = selectedTile;

    // The keys and their corresponding actions
    std::vector<int> keys = {GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_DOWN, GLFW_KEY_UP};

    std::vector<glm::vec2> actions = {
        glm::vec2(selectedTile.x - 1, selectedTile.y),
        glm::vec2(selectedTile.x + 1, selectedTile.y),
        glm::vec2(selectedTile.x, selectedTile.y - 1),
        glm::vec2(selectedTile.x, selectedTile.y + 1)
    };


    // Decide between either checking for a new key to be pressed
    // or for an already pressed key to be released

    bool checkReleased = isPressed;
    isPressed = false;

    for (int i = 0; i < keys.size(); ++i) {
        // Checks if a new key has been pressed, and updates selectedTile
        if (!checkReleased && (glfwGetKey(window, keys[i]) == GLFW_PRESS)) { selectedTile = actions[i]; isPressed = true;}

        // Ensures that a key input is only read once
        if (checkReleased && (glfwGetKey(window, keys[i]) != GLFW_RELEASE)) { isPressed = true; }
    }


    // The selected tile won't move if it would have moved outside the chess board
    if (selectedTile.x < 0 || selectedTile.y < 0 || selectedTile.x >= gridSize.x || selectedTile.y >= gridSize.y) { selectedTile = prevTile; }

    return {selectedTile, isPressed};
}


glm::vec3 scale(float scaleFactor) {
    return {scaleFactor, scaleFactor, scaleFactor};
}


// Transforms the projection, view, and model matrix with the wanted variables, and combines them into
// the transformation matrix that is to be used on the object during rendering
glm::mat4 transformationMatrix(glm::vec2 rotation) {

    // Projection matrix
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, -10.0f);



    // View matrix
    glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f); // Camera position
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);   // Camera's target point
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);      // Up vector

    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, upVector);



    // Model matrix (Identity matrix)
    glm::mat4 identityMatrix(1.0f);

    // Translation
    identityMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

    // Rotation

    // Rotate along the x-axis
    identityMatrix = glm::rotate(identityMatrix, rotation.x, {0.0f, 1.0f, 0.0f});

    // Rotate along the y-axis
    identityMatrix = glm::rotate(identityMatrix, rotation.y, {1.0f, 0.0f, 0.0f});

    // Scaling
    identityMatrix = glm::scale(identityMatrix, scale(1.0f));


    // Update matrix with the transformation

    return projectionMatrix * viewMatrix * identityMatrix;

}
