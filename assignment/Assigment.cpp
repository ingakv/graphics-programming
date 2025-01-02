#include "Assigment.h"
#include "GeometricTools/GeometricTools.h"
#include "Rendering/RenderCommands.h"
#include "GLFWApplication/errorHandling.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Shaders/Shader.h"
#include "Textures/TextureManager.h"
#include "Camera/PerspectiveCamera.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>

using namespace GeometricTools;
using namespace RenderCommands;


enum Unit {Red, Blue, Empty};

// The number of tiles in the x and y direction of the board
const int BOARDSIZE = 8;

std::pair<glm::vec2, bool> select_tile_key(GLFWwindow *window, std::pair<glm::vec2, bool> selectedTile, glm::vec2 gridSize);
std::pair<glm::vec2, bool>
select_cube_key(GLFWwindow *window, glm::vec<2, int> selectedTile, std::pair<glm::vec<2, int>, bool> selectedCube,
                Unit chessBoard[8][8]);
glm::vec3 rotate_key(GLFWwindow *window, glm::vec3 r);
float zoom_key(GLFWwindow *window, float zoom);
std::pair<bool, bool> texture_key(GLFWwindow *window, std::pair<bool, bool> applyTex);
Unit** updateChessboard(Unit chessBoard[BOARDSIZE][BOARDSIZE], glm::vec<2, int> moveCubeTo, glm::vec<2, int> moveCubeFrom);



int assignment_App::run() {


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
    auto lab_name = "assignment";
    auto gridShaderProgram = std::make_shared<Shader>(lab_name, "grid");
    auto cubeShaderProgram = std::make_shared<Shader>(lab_name, "cube");

    //////////////// Grid //////////////////

    int x,y;
    x = y = BOARDSIZE;

    // The layout of the grid
    auto grid = UnitGridGeometry2D(x, y);

    // The grid indices
    auto grid_indices = UnitGridTopologyTriangles(x,y);

    // Create buffers and arrays

    auto gridVertexArray = std::make_shared<VertexArray>();

    auto gridVertexBuffer = std::make_shared<VertexBuffer>(grid.data(),grid.size());

    auto gridBufferLayout = BufferLayout({
        {ShaderDataType::Float2, "position"}
    });

    auto gridIndexBuffer = std::make_shared<IndexBuffer>(grid_indices.data(), grid_indices.size(), gridBufferLayout);

    // Connect all the arrays
    gridVertexBuffer->SetLayout(gridBufferLayout);
    gridVertexArray->AddVertexBuffer(gridVertexBuffer);
    gridVertexArray->SetIndexBuffer(gridIndexBuffer);

    //////////////// Cube //////////////////

    // The layout of the cube
    auto cube = UnitCube3D24WNormals;

    // The cube indices
    auto cube_indices = UnitCube3DTopologyTriangles24;

    // Create buffers and arrays
    auto cubeVertexArray = std::make_shared<VertexArray>();


    auto cubeVertexBuffer = std::make_shared<VertexBuffer>(cube.data(), cube.size());

    auto cubeBufferLayout = BufferLayout({
        {ShaderDataType::Float3, "position"},
        {ShaderDataType::Float3, "normal"}
    });

    auto cubeIndexBuffer = std::make_shared<IndexBuffer>(cube_indices.data(), cube_indices.size(), cubeBufferLayout);


    // Connect all the arrays
    cubeVertexBuffer->SetLayout(cubeBufferLayout);
    cubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
    cubeVertexArray->SetIndexBuffer(cubeIndexBuffer);


    //////////////// Textures //////////////////

    auto filepath = std::string(TEXTURES_DIR) + std::string("floor_texture.png");

    auto textures = TextureManager::GetInstance();
    textures->LoadTexture2DRGBA("floor_texture", filepath, 0, false);


    filepath = std::string(TEXTURES_DIR) + std::string("cube_texture.png");

    textures = TextureManager::GetInstance();
    textures->LoadCubeMapRGBA("cube_texture", filepath, 1, false);

    ////////////////////////////////////////


    // The selected tile
    // x & y = the coordinates
    // z = whether a key is pressed
    std::pair<glm::vec2, bool> selectedTile = {{0, 0}, false};

    // The selected cube / The position where a cube is to be moved from
    // x & y = the coordinates
    // z = whether a key is pressed
    std::pair<glm::vec2, bool> selectedCube = {{-1, -1}, false};

    // The board's rotation
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};

    // If texture is going to be applied
    std::pair applyTexture = {false, false};

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Enable blending
    glEnable(GL_BLEND);

    // Set the blending function: s*alpha + d(1-alpha)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::vec4 backgroundColor = glm::vec4(0.5f, 0.3f, 0.7f, 1.0f);

    double currentTime;
    glfwSetTime(0.0);

    // Transformation matrix
    glm::vec3 cameraPosition(0.0f, -1.0f, 1.0f); // Camera position
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);   // Camera's target point
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);      // Up vector

    PerspectiveCamera::Frustrum frustrum = {90.0f, width, height, 0.1f, -10.0f, scale(1.0f), rotation};

    auto camera = PerspectiveCamera(frustrum, cameraPosition, cameraTarget, upVector);


    // Initialize the board
    Unit chessBoard[BOARDSIZE][BOARDSIZE];

    for (int j = 0; j < BOARDSIZE; ++j) {
        for (auto & i : chessBoard) { i[j] = Empty; }

        // The first two rows of cubes
        chessBoard[0][j] = Red;
        chessBoard[1][j] = Red;

        // The last two rows of cubes
        chessBoard[BOARDSIZE - 1][j] = Blue;
        chessBoard[BOARDSIZE - 2][j] = Blue;
    }

    // Upload the textures
    gridShaderProgram->Bind();
    gridShaderProgram->UploadUniformInt("uTexture", 0);
    gridShaderProgram->Unbind();

    cubeShaderProgram->Bind();

    // Upload the texture
    cubeShaderProgram->UploadUniformInt("uTexture", 1);

    // Upload the ambient light
    cubeShaderProgram->UploadUniformFloat3("u_ambientColor", backgroundColor);

    // Upload the camera position
    cubeShaderProgram->UploadUniformFloat3("u_lightSourcePosition", glm::vec3 (0.0, 0.0, 1.0));
    cubeShaderProgram->UploadUniformFloat3("u_cameraPosition", cameraPosition);

    // Upload the diffuse color and strength factor
    cubeShaderProgram->UploadUniformFloat3("u_diffuseColor", backgroundColor);

    // Upload the specular color and strength factor
    cubeShaderProgram->UploadUniformFloat3("u_specularColor", backgroundColor);
    cubeShaderProgram->UploadUniformInt("u_shininessFactor", 100);

    cubeShaderProgram->Unbind();

    // Application loop code
    while (!glfwWindowShouldClose(window)) {

        // The currently or newly selected tile
        selectedTile = select_tile_key(window, selectedTile, glm::vec2(x,y));
        selectedCube = select_cube_key(window, selectedTile.first, selectedCube, chessBoard);

        // Apply / remove texture if the T key is pressed
        applyTexture = texture_key(window, applyTexture);

        // Update board rotation
        frustrum.rotation = rotate_key(window, frustrum.rotation);

        // Update the zoom
        frustrum.angle = zoom_key(window, frustrum.angle);
        camera.SetFrustrum(frustrum);

        // Update time
        currentTime = glfwGetTime();

        // Clear the screen
        Clear();

        // Dynamically changes the light between 0.5f and 1.0f
        float dynamicLight = (sin(currentTime/5) + 3.0f) / 4.0f;

        // Background color
        SetClearColor(backgroundColor * dynamicLight);

        // Unit cube
        cubeVertexArray->Bind();
        cubeShaderProgram->Bind();

        // Transform the cube into the correct transformation matrix
        auto cubeCamera = camera;
        auto cubeFrustrum = frustrum;

        // Make the cubes 70% of the size of a square on the board
        cubeFrustrum.scaling = scale((1.0f / BOARDSIZE) * 0.7);
        cubeCamera.SetFrustrum(cubeFrustrum);

        // Update whether the texture should be shown
        cubeShaderProgram->UploadUniformInt("showTexture", applyTexture.first);

        // Update the selected tile
        cubeShaderProgram->UploadUniformFloat2("selectedSquare", selectedTile.first);

        // Update the selected cube
        cubeShaderProgram->UploadUniformFloat2("selectedCube", selectedCube.first);

        cubeShaderProgram->UploadUniformMatrix4fv("u_TransformationMat", cubeCamera.GetViewProjectionMatrix());
        SetSolidMode();

        for (int j = 0; j < BOARDSIZE; ++j) {
            for (int i = 0; i < BOARDSIZE; ++i) {

                if (chessBoard[i][j] != Empty) {

                    (chessBoard[i][j] == Red) ? cubeShaderProgram->UploadUniformInt("redTeam", 1) : cubeShaderProgram->UploadUniformInt("redTeam", 0);

                    cubeShaderProgram->UploadUniformFloat2("cubePosition", glm::vec2(j, i));
                    DrawIndex(cubeVertexArray, GL_TRIANGLES);

                }

            }
        }

        // Grid
        gridShaderProgram->Bind();
        gridVertexArray->Bind();

        // Update the selected tile
        gridShaderProgram->UploadUniformFloat2("selectedSquare", selectedTile.first);

        // Transform the grid into the correct transformation matrix
        gridShaderProgram->UploadUniformMatrix4fv("u_TransformationMat", camera.GetViewProjectionMatrix());

        // Update whether the texture should be shown
        gridShaderProgram->UploadUniformInt("showTexture", applyTexture.first);

        DrawIndex(gridVertexArray, GL_TRIANGLES);

        // Poll for events (like input)
        glfwPollEvents();

        // Swap buffers to show our shape
        glfwSwapBuffers(window);

        // The window closes when the Q key is pressed
        if (glfwGetKey(window, GLFW_KEY_Q) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        break;

    }

    gridShaderProgram->Unbind();
    cubeShaderProgram->Unbind();

    return EXIT_SUCCESS;

}



// Switches between showing the textures or not if the T key is pressed
std::pair<bool, bool> texture_key(GLFWwindow *window, std::pair<bool, bool> applyTex) {

    // Decide between either checking for a new key to be pressed
    // or for an already pressed key to be released

    // Checks if a new key has been pressed, and updates the bool
    if (!applyTex.second && (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)) { return {!applyTex.first, true}; }

    // Ensures that a key input is only read once
    if (applyTex.second && (glfwGetKey(window, GLFW_KEY_T) != GLFW_RELEASE)) { return {applyTex.first, true}; }

    return {applyTex.first, false};

}

float zoom_key(GLFWwindow *window, float zoom) {

    float zoomSpeed = 1.0f;

    auto prevZoom = zoom;

    // The keys and their corresponding actions
    std::vector<std::pair<int, float>> key_actions = {
        {GLFW_KEY_O,
         {zoom + zoomSpeed}},

        {GLFW_KEY_P,
         {zoom - zoomSpeed}},

        {GLFW_KEY_DOWN,
         {zoom + zoomSpeed}},

        {GLFW_KEY_UP,
         {zoom - zoomSpeed}}
    };

    // Checks if one of the keys are pressed, and updates the rotation and zoom values
    for (auto & item : key_actions) {
        if (glfwGetKey(window, item.first) == GLFW_PRESS) { zoom = item.second; }
    }

    // Does not update the zoom value if it is outside the min and max values
    if (zoom > 90.0f || zoom < 10.0f) zoom = prevZoom;

    return zoom;
}


// rotation.x, rotation.y & rotation.z = how many degrees of rotation per axis
glm::vec3 rotate_key(GLFWwindow *window, glm::vec3 r) {

    float rotateSpeed = 0.02f;

    // The keys and their corresponding actions
    std::vector<std::pair<int, glm::vec3>> key_actions = {
        {GLFW_KEY_H,
         {r.x, r.y, r.z + rotateSpeed}},

        {GLFW_KEY_L,
         {r.x, r.y, r.z - rotateSpeed}},

        {GLFW_KEY_LEFT,
         {r.x, r.y, r.z + rotateSpeed}},

        {GLFW_KEY_RIGHT,
         {r.x, r.y, r.z - rotateSpeed}}
    };

    // Checks if one of the keys are pressed, and updates the rotation and zoom values
    for (auto & item : key_actions) {
        if (glfwGetKey(window, item.first) == GLFW_PRESS) { return item.second; }
    }

    return r;
}


std::pair<glm::vec2, bool> select_tile_key(GLFWwindow *window, std::pair<glm::vec2, bool> selectedTile, glm::vec2 gridSize) {

    auto prevTile = selectedTile.first;

    // The keys and their corresponding actions
    std::vector<std::pair<int, glm::vec2>> key_actions = {
        {GLFW_KEY_A,
        {prevTile.x - 1, prevTile.y}},

        {GLFW_KEY_D,
        {prevTile.x + 1, prevTile.y}},

        {GLFW_KEY_S,
        {prevTile.x, prevTile.y - 1}},

        {GLFW_KEY_W,
        {prevTile.x, prevTile.y + 1}}
    };

    bool isPressed = selectedTile.second;
    selectedTile.second = false;

    // Decide between either checking for a new key to be pressed
    // or for an already pressed key to be released
    for (auto & item : key_actions) {

        // Checks if a new key has been pressed, and updates selectedTile
        if (!isPressed && (glfwGetKey(window, item.first) == GLFW_PRESS)) { selectedTile = {item.second, true};}

        // Ensures that a key input is only read once
        if (isPressed && (glfwGetKey(window, item.first) != GLFW_RELEASE)) { selectedTile.second = true; }

    }


    // The selected tile won't move if it would have moved outside the chess board
    if (selectedTile.first.x < 0 || selectedTile.first.y < 0 || selectedTile.first.x >= gridSize.x || selectedTile.first.y >= gridSize.y) { selectedTile.first = prevTile; }

    return selectedTile;
}

// Update where the cube should be moved to and from
Unit** updateChessboard(Unit chessBoard[BOARDSIZE][BOARDSIZE], glm::vec<2, int> moveCubeTo, glm::vec<2, int> moveCubeFrom) {

    std::vector pos = {moveCubeFrom.x, moveCubeFrom.y, moveCubeTo.x, moveCubeTo.y};

    // If not all the variables are initialized, don't update the chessboard
    if(std::find(pos.begin(), pos.end(), -1) != pos.end()) return reinterpret_cast<Unit **>(chessBoard);

    // Move the cube to the new unit
    chessBoard[moveCubeTo.y][moveCubeTo.x] = chessBoard[moveCubeFrom.y][moveCubeFrom.x];

    // Clear the old unit
    chessBoard[moveCubeFrom.y][moveCubeFrom.x] = Empty;

    return reinterpret_cast<Unit **>(chessBoard);

}


std::pair<glm::vec2, bool> select_cube_key(GLFWwindow *window, glm::vec<2, int> selectedTile, std::pair<glm::vec<2, int>, bool> selectedCube, Unit chessBoard[8][8]) {

    bool isPressed = selectedCube.second;
    selectedCube.second = false;

    bool isEmpty = chessBoard[selectedTile.y][selectedTile.x] == Empty;

    // Decide between either checking for a new key to be pressed
    // or for an already pressed key to be released

    // Checks if a new key has been pressed, and updates the selected cube
    if (!isPressed && (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)) {

        // If no cube has been selected yet, update it and the selectedCube variable
        if (selectedCube.first == glm::vec<2, int>(-1,-1) && !isEmpty) {
            selectedCube.first = selectedTile;
        }

        // If the cube is to be moved, reset the selectedCube variable and update the chessboard
        else {
            if (isEmpty) { updateChessboard(chessBoard, selectedTile, selectedCube.first); }

            // If the unit where the cube should be moved is occupied, cancel the movement
            selectedCube.first = {-1, -1};
        }

        selectedCube.second = true;
    }

    // Ensures that a key input is only read once
    if (isPressed && (glfwGetKey(window, GLFW_KEY_ENTER) != GLFW_RELEASE)) { selectedCube.second = true; }


    return selectedCube;

}
