#include "Exam.h"
#include "GeometricTools/GeometricTools.h"
#include "Rendering/RenderCommands.h"
#include "GLFWApplication/errorHandling.h"
#include "Shaders/Shader.h"
#include "Textures/TextureManager.h"
#include "Camera/PerspectiveCamera.h"

#include <random>
#include <vector>
#include <algorithm>

using namespace GeometricTools;
using namespace RenderCommands;


enum Object {Wall, Player, Box, Target, Empty};

enum Direction {Left, Right, Up, Down};

// The number of tiles in the x and y direction of the board
const int BOARDSIZE = 10;

// Ongoing board movements
std::vector<std::pair<glm::vec4, double>> movements;

// The regular board
Object board[BOARDSIZE][BOARDSIZE];

// The separate board with targets, which allows other objects to "pass through" the targets
Object targetBoard[BOARDSIZE][BOARDSIZE];



glm::vec3 rotate_key(GLFWwindow *window, glm::vec3 r);
float zoom_key(GLFWwindow *window, float zoom);
std::pair<bool, bool> texture_key(GLFWwindow *window, std::pair<bool, bool> applyTex);
glm::vec<2, int> moveDirections(glm::vec2 oldPos, Direction direction);
void addRandomToBoard(Object objectType);
glm::vec2 getPlayerPosition();
bool checkBoxTarget(glm::vec<2, int> pos);
void findTexture(const std::shared_ptr<Shader>& shaderProgram, Object objectType);
void moveObjectOnBoard(glm::vec<2, int> moveCubeFrom, Direction direction, double curTime);
bool move_player_key(GLFWwindow *window, bool playerMoved, double curTime);
void moveObject(double curTime, glm::vec<2, int> moveCubeFrom, glm::vec<2, int> moveCubeTo);


int exam_App::run() {


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
    auto lab_name = "exam";
    auto gridShaderProgram = std::make_shared<Shader>(lab_name, "grid");
    auto cubeShaderProgram = std::make_shared<Shader>(lab_name, "cube");

    //////////////// Grid / Floor //////////////////

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



    //////////////// Walls //////////////////

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




    //////////////// Initialize the boards //////////////////

    for (int j = 0; j < BOARDSIZE; ++j) {
        for (int i = 0; i < BOARDSIZE; ++i) {

            // Place walls on the edges of the board, and make the rest empty

            board[i][j] = (
                i == BOARDSIZE - 1 ||
                j == BOARDSIZE - 1 ||
                i == 0 ||
                j == 0
            ) ? Wall : Empty;

            targetBoard[i][j] = Empty;

        }
    }

    //////////////// Add objects to random empty tiles //////////////////

    // The player
    addRandomToBoard(Player);

    // The boxes
    for (int i = 0; i < 6; ++i) {
        addRandomToBoard(Box);
    }

    // The extra wall blocks
    for (int i = 0; i < 6; ++i) {
        addRandomToBoard(Wall);
    }

    // The target locations
    for (int i = 0; i < 6; ++i) {
        addRandomToBoard(Target);
    }




    //////////////// Textures //////////////////

    auto textures = TextureManager::GetInstance();



    auto filepath = std::string(TEXTURES_DIR) + std::string("floor_texture.png");
    textures->LoadTexture2DRGBA("floor", filepath, 0, false);


    filepath = std::string(TEXTURES_DIR) + std::string("steve.png");
    textures->LoadCubeMapRGBA("player", filepath, 1, false);


    filepath = std::string(TEXTURES_DIR) + std::string("box.png");
    textures->LoadCubeMapRGBA("box", filepath, 2, false);


    filepath = std::string(TEXTURES_DIR) + std::string("wall.png");
    textures->LoadCubeMapRGBA("wall", filepath, 3, false);


    ////////////////////////////////////////

    // Whether an arrow key is pressed
    bool playerMoved = false;

    // The board's rotation
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};

    // If texture is going to be applied
    std::pair applyTexture = {false, false};

    // The cube movement animation speed
    double cubeSpeed = 3;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Enable blending
    glEnable(GL_BLEND);

    // Set the blending function: s*alpha + d(1-alpha)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Background color
    glm::vec4 backgroundColor = glm::vec4(0.5f, 0.3f, 0.7f, 1.0f) * 0.5f;

    double currentTime;
    glfwSetTime(0.0);


    //////////////// Camera //////////////////

    // Transformation matrix
    glm::vec3 cameraPosition(0.0f, -0.75f, 1.0f); // Camera position
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);   // Camera's target point
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);      // Up vector

    PerspectiveCamera::Frustrum frustrum = {90.0f, width, height, 0.1f, -10.0f, scale(0.99f), rotation};

    auto camera = PerspectiveCamera(frustrum, cameraPosition, cameraTarget, upVector);



    //////////////// Upload to the shaders //////////////////

    
    glm::vec4 lightColor = glm::vec4 (scale(1.0f), 1.5f);
    
    
    for (auto & shader : {gridShaderProgram, cubeShaderProgram}) {

        shader->Bind();

        // Upload the board size
        shader->UploadUniformFloat("boardSize", 10);

        // Upload the camera position
        shader->UploadUniformFloat3("u_lightSourcePosition", cameraPosition);
        shader->UploadUniformFloat3("u_cameraPosition", cameraPosition);

        // Upload the ambient light
        shader->UploadUniformFloat3("u_ambientColor", lightColor);

        // Upload the diffuse color and strength factor
        shader->UploadUniformFloat3("u_diffuseColor", lightColor);
        shader->UploadUniformFloat("u_diffuseStrength", 1.0f);

        // Upload the specular color and strength factor
        shader->UploadUniformFloat3("u_specularColor", lightColor);
        shader->UploadUniformFloat("u_specularStrength", 0.5f);


        // Upload the texture
        findTexture(gridShaderProgram, Empty);

        shader->Unbind();
        
    }


    //////////////// Application loop code //////////////////

    while (!glfwWindowShouldClose(window)) {

        // Update time
        currentTime = glfwGetTime();

        // Clear the screen
        Clear();

        // Move the player if one of the correct keys is pressed
        playerMoved = move_player_key(window, playerMoved, currentTime);

        // Apply / remove texture if the T key is pressed
        applyTexture = texture_key(window, applyTexture);

        // Update board rotation
        frustrum.rotation = rotate_key(window, frustrum.rotation);

        // Update the zoom
        frustrum.angle = zoom_key(window, frustrum.angle);
        camera.SetFrustrum(frustrum);

        // Dynamically changes the light between 0.5f and 1.0f
        float dynamicLight = (sin(currentTime) + 3.0f) / 4.0f;

        // Background color
        SetClearColor(backgroundColor);



        // Transform the cube into the correct transformation matrix
        auto cubeCamera = camera;
        auto cubeFrustrum = frustrum;

        // Make each cube the size of a tile on the board
        cubeFrustrum.scaling = scale((1.0f / BOARDSIZE) * 0.99f);
        cubeCamera.SetFrustrum(cubeFrustrum);

        auto playerCubeCamera = cubeCamera;
        auto playerCubeFrustrum = cubeFrustrum;

        // Make the player cube smaller
        playerCubeFrustrum.scaling = scale((1.0f / BOARDSIZE) * 0.7);
        playerCubeCamera.SetFrustrum(playerCubeFrustrum);


        auto wallCubeCamera = cubeCamera;
        auto wallCubeFrustrum = cubeFrustrum;

        // Make the wall cubes shorter
        wallCubeFrustrum.scaling.z /= 2;
        wallCubeCamera.SetFrustrum(wallCubeFrustrum);





        //////////////// The cubes //////////////////

        cubeVertexArray->Bind();
        cubeShaderProgram->Bind();

        // Update whether the texture should be shown
        cubeShaderProgram->UploadUniformInt("showTexture", applyTexture.first);


        // Loop through the board
        for (int j = 0; j < BOARDSIZE; ++j) {

            for (int i = 0; i < BOARDSIZE; ++i) {

                auto cubePos = glm::vec2(j, i);

                // Update the cube size
                cubeShaderProgram->UploadUniformMatrix4fv("u_TransformationMat", cubeCamera.GetViewProjectionMatrix());

                // Upload the ambient strength factor
                cubeShaderProgram->UploadUniformFloat("u_ambientStrength", 0.5f);

                // Upload what the object type is
                cubeShaderProgram->UploadUniformInt("objectType", board[i][j]);

                if (board[i][j] != Empty) {

                    // Check for active movements
                    for (auto & move : movements) {

                        if (glm::vec2(move.first) == glm::vec2(i, j)) {

                            // Show the cube's "animated" movement instead of its actual position on the tiles
                            cubePos.x = move.first.w + (cubePos.x - move.first.w) * sin(cubeSpeed*(currentTime - move.second));
                            cubePos.y = move.first.z +(cubePos.y - move.first.z) * sin(cubeSpeed*(currentTime - move.second));

                        }

                        // After about a second, the movement animation will be over
                        else if (currentTime - move.second >= 1.5/cubeSpeed) { movements.pop_back(); }

                    }

                    // Update cube position
                    cubeShaderProgram->UploadUniformFloat2("cubePosition", cubePos);

                    // Upload the texture
                    findTexture(cubeShaderProgram, board[i][j]);



                    // Walls = 0
                    // Player = 1
                    // Box = 2
                    // Target = 3
                    // Box on target = 4

                    // Distinguish between the object types
                    switch(board[i][j]) {
                        case Player: {

                            // Make the player cube smaller
                            cubeShaderProgram->UploadUniformMatrix4fv("u_TransformationMat", playerCubeCamera.GetViewProjectionMatrix());

                            // Upload the ambient strength factor of the player cube to make a glowing effect
                            cubeShaderProgram->UploadUniformFloat("u_ambientStrength", dynamicLight);

                        } break;

                            // Distinguish between correctly and incorrectly placed boxes
                        case Box: { if (checkBoxTarget({i, j})) cubeShaderProgram->UploadUniformInt("objectType", 4); } break;

                            // Make the wall cubes shorter
                        case Wall: { cubeShaderProgram->UploadUniformMatrix4fv("u_TransformationMat", wallCubeCamera.GetViewProjectionMatrix()); } break;

                        default: break;
                    }

                    DrawIndex(cubeVertexArray, GL_TRIANGLES);

                }




                if (targetBoard[i][j] != Empty) {

                    // Update the cube size
                    cubeShaderProgram->UploadUniformMatrix4fv("u_TransformationMat", cubeCamera.GetViewProjectionMatrix());

                    // Upload the ambient strength factor
                    cubeShaderProgram->UploadUniformFloat("u_ambientStrength", 0.5f);

                    // Upload what the object type is
                    cubeShaderProgram->UploadUniformInt("objectType", targetBoard[i][j]);

                    // Update cube position
                    cubeShaderProgram->UploadUniformFloat2("cubePosition", glm::vec2(j, i));

                    DrawIndex(cubeVertexArray, GL_TRIANGLES);

                }

            }
        }



        // Grid
        gridShaderProgram->Bind();
        gridVertexArray->Bind();

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



// Adds an instance of the object type to the board at a random empty tile
void addRandomToBoard(Object objectType) {

    // Random number generator
    std::random_device dev;
    std::mt19937 rng(dev());

    // Create a vector of all the empty spaces on the board
    std::vector<std::pair<int,int>> freeSpaces;


    for (int j = 0; j < BOARDSIZE; ++j) {
        for (int i = 0; i < BOARDSIZE; ++i) {
            // Ensures that target cubes won't be initially placed on top of other cubes
            if (board[i][j] == Empty && targetBoard[i][j] == Empty)
                freeSpaces.push_back(std::pair(i,j));
        }
    }


    // Pick a random index from the vector of empty spaces
    std::uniform_int_distribution<int> randomEmptySpace(0,freeSpaces.size()-1);


    auto playerPos = randomEmptySpace(rng);

    if (objectType == Target) targetBoard[freeSpaces[playerPos].first][freeSpaces[playerPos].second] = objectType;

    else board[freeSpaces[playerPos].first][freeSpaces[playerPos].second] = objectType;

}


// Returns the tile index of where the player currently is at
glm::vec2 getPlayerPosition() {

    auto playerPos = glm::vec2 (0, 0);

    for (int j = 0; j < BOARDSIZE; ++j) {
        for (int i = 0; i < BOARDSIZE; ++i) {
            if (board[i][j] == Player) { playerPos = glm::vec2(i, j); }
        }
    }

    return playerPos;
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
        {GLFW_KEY_S,
         {zoom + zoomSpeed}},

        {GLFW_KEY_W,
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
        {GLFW_KEY_A,
         {r.x, r.y, r.z + rotateSpeed}},

        {GLFW_KEY_D,
         {r.x, r.y, r.z - rotateSpeed}}
    };

    // Checks if one of the keys are pressed, and updates the rotation and zoom values
    for (auto & item : key_actions) {
        if (glfwGetKey(window, item.first) == GLFW_PRESS) { return item.second; }
    }

    return r;
}

glm::vec<2, int> moveDirections(glm::vec2 oldPos, Direction direction) {
    
    glm::vec<2,int> newPos{};
    
    // Calculates the new position based on what direction the cube should be moved in
    switch (direction) {
        case Left: newPos = {oldPos.x, oldPos.y - 1};
            break;
        case Right: newPos = {oldPos.x, oldPos.y + 1};
            break;
        case Down: newPos = {oldPos.x - 1, oldPos.y};
            break;
        case Up: newPos = {oldPos.x + 1, oldPos.y};
            break;
        default: newPos = oldPos;
    }

    return newPos;
    
}


// Checks if a box is in a target location
bool checkBoxTarget(glm::vec<2, int> pos) {

    if (targetBoard[pos.x][pos.y] == Target && board[pos.x][pos.y] == Box)
        return true;

    return false;

}


void moveObject(double curTime, glm::vec<2, int> moveCubeFrom, glm::vec<2, int> moveCubeTo) {

    // The cube won't move if it would have moved outside the chess board
    // or into a box or wall
    if (!(moveCubeTo.x < 0 || moveCubeTo.x >= BOARDSIZE ||
        moveCubeTo.y < 0 || moveCubeTo.y >= BOARDSIZE ||
        (board[moveCubeTo.x][moveCubeTo.y] != Empty && board[moveCubeTo.x][moveCubeTo.y] != Target))) {

        // Move the cube to the new tile
        board[moveCubeTo.x][moveCubeTo.y] = board[moveCubeFrom.x][moveCubeFrom.y];

        // Clear the old tile
        board[moveCubeFrom.x][moveCubeFrom.y] = Empty;

        // Updates the board movements
        movements.insert(movements.begin(), std::pair(glm::vec4(moveCubeTo, moveCubeFrom), curTime));

    }

}


// Update where the cube should be moved to and from
void moveObjectOnBoard(glm::vec<2, int> moveCubeFrom, Direction direction, double curTime) {

    // Find the tile next to the object
    auto nextTile = moveDirections(moveCubeFrom, direction);

    // Find the next tile after that
    auto nextNextTile = moveDirections(nextTile, direction);

    // Calculates the new position based on what direction the cube should be moved in
    switch (board[nextTile.x][nextTile.y]) {
        case Box:

            // Move the box to the new tile
            moveObject(curTime, nextTile, nextNextTile);

            // Move the player to the box's previous position
            moveObject(curTime, getPlayerPosition(), nextTile);

            break;

        case Empty:
            moveObject(curTime, moveCubeFrom, nextTile);
            break;
        default:
            break;
    }

}


// Uploads the correct texture according to the object type
void findTexture(const std::shared_ptr<Shader>& shaderProgram, Object objectType) {
    
    std::string name;

    // Convert to string
    switch (objectType) {
        case Player: name = "player"; break;
        case Box: name = "box"; break;
        case Empty: name = "floor_texture"; break;
        default: name = "wall"; break;
    }

    // Upload to the shader
    shaderProgram->UploadUniformInt("uTexture", TextureManager::GetInstance()->GetUnitByName(name));

}

bool move_player_key(GLFWwindow *window, bool playerMoved, double curTime) {

    auto prevPos = getPlayerPosition();

    // The keys and their corresponding actions
    std::vector<std::pair<int, Direction>> key_actions = {

        {GLFW_KEY_LEFT,Left},

        {GLFW_KEY_RIGHT,Right},

        {GLFW_KEY_DOWN,Down},

        {GLFW_KEY_UP,Up}
        
    };

    bool isPressed = playerMoved;
    playerMoved = false;

    // Decide between either checking for a new key to be pressed
    // or for an already pressed key to be released
    for (auto & item : key_actions) {

        // Checks if a new key has been pressed, and updates the board
        if (!isPressed && (glfwGetKey(window, item.first) == GLFW_PRESS)) {

            // Update the board
            moveObjectOnBoard(prevPos, item.second, curTime);

            playerMoved = true;

        }

        // Ensures that a key input is only read once
        if (isPressed && (glfwGetKey(window, item.first) != GLFW_RELEASE)) { playerMoved = true; }

    }

    return playerMoved;

}

