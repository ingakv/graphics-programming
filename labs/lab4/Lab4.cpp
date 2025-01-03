#include "Lab4.h"
#include "GeometricTools/GeometricTools.h"
#include "Rendering/RenderCommands.h"
#include "GLFWApplication/errorHandling.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Shaders/Shader.h"
#include "Textures/TextureManager.h"
#include "Camera/PerspectiveCamera.h"
#include "Camera/OrthographicCamera.h"


#include <iostream>
#include <string>
#include <cstdlib>


using namespace GeometricTools;
using namespace RenderCommands;

glm::vec3 select_tile_key(GLFWwindow *window, glm::vec2 selectedTile, bool isPressed, glm::vec2 gridSize);
glm::vec3 rotate_cube_key(GLFWwindow *window, glm::vec3 rotation);


int lab4_App::run() {


    // Enabling capture of debug output to get messages about potential issues.
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // Print OpenGL context information.
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";


    //////////////// Shaders //////////////////
    auto gridShaderProgram = std::make_shared<Shader>("labs", "grid");
    auto cubeShaderProgram = std::make_shared<Shader>("lab4", "cube");

    //////////////// Grid //////////////////

    int x,y;
    x = y = 8;


    // The layout of the grid
    auto grid = UnitGridGeometry2D(x, y);


    // The grid indices
    auto grid_indices = UnitGridTopologyTriangles(x,y);

    // Create buffers and arrays

    auto gridVertexArray = std::make_shared<VertexArray>();


    auto gridVertexBuffer = std::make_shared<VertexBuffer>(grid.data(), grid.size());

    auto gridBufferLayout = BufferLayout({ {ShaderDataType::Float2, "position"} });

    auto gridIndexBuffer = std::make_shared<IndexBuffer>(grid_indices.data(), grid_indices.size(), gridBufferLayout);




    // Connect all the arrays
    gridVertexBuffer->SetLayout(gridBufferLayout);
    gridVertexArray->AddVertexBuffer(gridVertexBuffer);
    gridVertexArray->SetIndexBuffer(gridIndexBuffer);



    //////////////// Cube //////////////////

    // The layout of the cube
    auto cube = UnitCube;


    // The cube indices
    auto cube_indices = UnitCubeTopologyTriangles;

    // Create buffers and arrays
    auto cubeVertexArray = std::make_shared<VertexArray>();


    auto cubeVertexBuffer = std::make_shared<VertexBuffer>(cube.data(), cube.size());

    auto cubeBufferLayout = BufferLayout({{ShaderDataType::Float3, "position"}});

    auto cubeIndexBuffer = std::make_shared<IndexBuffer>(cube_indices.data(), cube_indices.size(), cubeBufferLayout);



    // Connect all the arrays
    cubeVertexBuffer->SetLayout(cubeBufferLayout);
    cubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
    cubeVertexArray->SetIndexBuffer(cubeIndexBuffer);


    //////////////// Textures //////////////////

    auto filepath = std::string(TEXTURES_DIR) + std::string("gatos.png");

    auto textures = TextureManager::GetInstance();
    textures->LoadCubeMapRGBA("gato_cube", filepath, 0, false);

    ////////////////////////////////////////






    // The default selected tile
    auto selectedTile = glm::vec2(0, 0);

    // The cube's rotation
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};

    // Whether a key is pressed
    bool isPressed = false;




    // Transformation matrix
    glm::vec3 cameraPosition(0.0f, 0.0f, 2.0f); // Camera position
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);   // Camera's target point
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);      // Up vector

    PerspectiveCamera::Frustrum perspectiveFrustrum = {90.0f, width, height, 1.0f, -10.0f, scale(1.0f), rotation};
    OrthographicCamera::Frustrum orthographicFrustrum = {-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f, rotation};

    auto perspectiveCamera = PerspectiveCamera(perspectiveFrustrum, cameraPosition, cameraTarget, upVector);
    auto orthographicCamera = OrthographicCamera(orthographicFrustrum, cameraPosition);


    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Enable blending
    glEnable(GL_BLEND);

    // Set the blending function: s*alpha + d(1-alpha)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Application loop code
    while (!glfwWindowShouldClose(window)) {

        // The currently or newly selected tile
        auto key_call_board = select_tile_key(window, selectedTile, isPressed, glm::vec2(x,y));
        selectedTile = glm::vec2(key_call_board.x, key_call_board.y);
        isPressed = key_call_board.z;

        // Update cube rotation
        perspectiveFrustrum.rotation = rotate_cube_key(window, perspectiveFrustrum.rotation);
        orthographicFrustrum.rotation = rotate_cube_key(window, orthographicFrustrum.rotation);

        perspectiveCamera.SetFrustrum(perspectiveFrustrum);
        orthographicCamera.SetFrustrum(orthographicFrustrum);


        // Poll for events (like input)
        glfwPollEvents();

        // Clear the screen
        Clear();

        // Background color
        SetClearColor(glm::vec4(0.5f, 0.3f, 0.7f, 1.0f));





        // Unit cube
        cubeVertexArray->Bind();
        cubeShaderProgram->Bind();

        // Transform the grid into the correct transformation matrix
        cubeShaderProgram->UploadUniformMatrix4fv("u_TransformationMat", perspectiveCamera.GetViewProjectionMatrix());

        // Unit cube outline
        SetWireframeMode();
        DrawIndex(cubeVertexArray, GL_TRIANGLES);


        // Upload the texture
        cubeShaderProgram->UploadUniformInt("uTexture", 0);

        SetSolidMode();

        glDisable(GL_DEPTH_TEST);
        DrawIndex(cubeVertexArray, GL_TRIANGLES);
        glEnable(GL_DEPTH_TEST);




        // Grid
        gridShaderProgram->Bind();
        gridVertexArray->Bind();

        // Upload the texture
        gridShaderProgram->UploadUniformInt("uTexture", 0);

        // Update the selected tile
        gridShaderProgram->UploadUniformFloat2("selectedSquare", selectedTile);

        // Transform the grid into the correct transformation matrix
        gridShaderProgram->UploadUniformMatrix4fv("u_TransformationMat", perspectiveCamera.GetViewProjectionMatrix());

//        DrawIndex(gridVertexArray, GL_TRIANGLES);





        // Swap buffers to show our shape
        glfwSwapBuffers(window);



        // The window closes when ESC is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;

    }



    gridShaderProgram->Unbind();
    cubeShaderProgram->Unbind();

    glfwTerminate();

    return  EXIT_SUCCESS;

}


// rotation.x & rotation.y = how many degrees of rotation per axis
glm::vec3 rotate_cube_key(GLFWwindow *window, glm::vec3 rotation) {

    float rotateSpeed = 0.05f;

    // The keys and their corresponding actions
    std::vector<int> keys = {GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_DOWN, GLFW_KEY_UP};

    std::vector<glm::vec3> actions = {
        {rotation.x - rotateSpeed, rotation.y, rotation.z},
        {rotation.x + rotateSpeed, rotation.y, rotation.z},
        {rotation.x, rotation.y + rotateSpeed, rotation.z},
        {rotation.x, rotation.y - rotateSpeed, rotation.z}
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
    std::vector<int> keys = {GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_S, GLFW_KEY_W};

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
