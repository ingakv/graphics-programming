#include "GLFWApplication/GLFWApplication.h"
#include "GeometricTools/GeometricTools.h"
#include "GLFWApplication/errorHandling.h"
#include "Lab1.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Shaders/Shader.h"


#include <iostream>
#include <memory>


using namespace GeometricTools;

glm::vec3 select_tile_key(GLFWwindow *window, glm::vec2 selectedTile, bool isPressed, glm::vec2 gridSize);

int lab1_App::run() {


    // Enabling capture of debug output to get messages about potential issues.
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // Print OpenGL context information.
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";


    auto shaderProgram = std::make_shared<Shader>("", "lab1");


    //////////////// Grid //////////////////

    int x,y;
    x = y = 8;


    // The layout of the grid
    auto grid = UnitGridGeometry2D(x, y);


    // The grid indices
    auto grid_indices = UnitGridTopologyTriangles(x, y);

    // Create buffers and arrays
    auto vertexArray = std::make_shared<VertexArray>();


    auto gridVertexBuffer = std::make_shared<VertexBuffer>(grid.data(), grid.size());


    auto gridBufferLayout = BufferLayout({{ShaderDataType::Float2, "position"}});

    auto gridIndexBuffer = std::make_shared<IndexBuffer>(grid_indices.data(), grid_indices.size(), gridBufferLayout);


    // Connect all the arrays
    gridVertexBuffer->SetLayout(gridBufferLayout);
    vertexArray->AddVertexBuffer(gridVertexBuffer);
    vertexArray->SetIndexBuffer(gridIndexBuffer);



    // The default selected tile
    auto selectedTile = glm::vec2(0, 0);

    // Whether a key is pressed
    bool isPressed = false;


    // Application loop code
    while (!glfwWindowShouldClose(window)) {

        // The currently or newly selected tile
        auto key_call = select_tile_key(window, selectedTile, isPressed, glm::vec2(x, y));

        selectedTile = glm::vec2(key_call.x, key_call.y);
        isPressed = key_call.z;


        // Poll for events (like input)
        glfwPollEvents();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Background color
        glClearColor(0.5f, 0.3f, 0.7f, 1.0f);



        // Grid
        shaderProgram->Bind();
        vertexArray->Bind();


        // Update the selected tile
        shaderProgram->UploadUniformFloat2("selectedSquare", selectedTile);

        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, (const void*)0);


        // Swap buffers to show our shape
        glfwSwapBuffers(window);

        // The window closes when ESC is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
    }



    shaderProgram->Unbind();

    glfwTerminate();

    return  EXIT_SUCCESS;

}

glm::vec3 select_tile_key(GLFWwindow *window, glm::vec2 selectedTile, bool isPressed, glm::vec2 gridSize) {

    glm::vec2 prevTile = selectedTile;


    // Ensures that a key input is only read once
    if (!isPressed) {

        isPressed = true;

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { selectedTile.x--; }

        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { selectedTile.x++; }

        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { selectedTile.y--; }

        else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { selectedTile.y++; }

        else { isPressed = false; }

    }

    else {
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) &&

        (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) &&

        (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) &&

        (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)) { isPressed = false; }
    }

    // The selected tile won't move if it would have moved outside the chess board
    if (selectedTile.x < 0 || selectedTile.y < 0 || selectedTile.x >= gridSize.x || selectedTile.y >= gridSize.y) { selectedTile = prevTile; }


    return {selectedTile, isPressed};
}
