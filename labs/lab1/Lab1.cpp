#include "Lab1.h"
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
#include <cstdlib>
#include <memory>

using namespace GeometricTools;


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

    auto shaderProgram = std::make_shared<Shader>("lab1", "");


    //////////////// Square //////////////////

    auto square = UnitSquare2D;
    auto square_indices = UnitGridTopologyTriangles(1, 1);

    auto squareVertexArray = std::make_shared<VertexArray>();

    auto squareVertexBuffer = std::make_shared<VertexBuffer>(square.data(), square.size());


    auto squareBufferLayout = BufferLayout({{ShaderDataType::Float2, "position"}});

    auto squareIndexBuffer = std::make_shared<IndexBuffer>(square_indices.data(), square_indices.size(), squareBufferLayout);


    // Connect all the arrays
    squareVertexBuffer->SetLayout(squareBufferLayout);
    squareVertexArray->AddVertexBuffer(squareVertexBuffer);
    squareVertexArray->SetIndexBuffer(squareIndexBuffer);



    //////////////// Triangle //////////////////

    auto triangle = UnitTriangle2D;
    auto triangles_indices = UnitTriangleTopology;

    auto triangleVertexArray = std::make_shared<VertexArray>();

    auto triangleVertexBuffer = std::make_shared<VertexBuffer>(triangle.data(), triangle.size());

    auto triangleBufferLayout = BufferLayout({{ShaderDataType::Float2, "position"}});

    auto triangleIndexBuffer = std::make_shared<IndexBuffer>(triangles_indices.data(), triangles_indices.size(), triangleBufferLayout);



    // Connect all the arrays
    triangleVertexBuffer->SetLayout(triangleBufferLayout);
    triangleVertexArray->AddVertexBuffer(triangleVertexBuffer);
    triangleVertexArray->SetIndexBuffer(triangleIndexBuffer);

    double currentTime;
    glfwSetTime(0.0);


    // Application loop code (SECTION 5)

    while (!glfwWindowShouldClose(window)) {

        // Poll for events (like input)
        glfwPollEvents();

        // Time management for alternate flag
        currentTime = glfwGetTime();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);


        //    This step fetches the location of the uniform variable "u_Color" within the shader program.

        auto trb = (sin(currentTime) / 2.0f) + 0.5f;
        auto sqg = (sin(currentTime) / 5.0f) + 0.5f;
        auto bgr = (sin(currentTime) / 5.0f) + 0.5f;


        // Background color
        glClearColor(bgr, 0.2f, 0.6f, 1.0f);


        // Square
        shaderProgram->Bind();
        squareVertexArray->Bind();
        shaderProgram->UploadUniformFloat4("u_Color",glm::vec4(1.0f, sqg, 0.6f, 1.0f));
        glDrawElements(GL_TRIANGLES, square.size(), GL_UNSIGNED_INT, nullptr);


        // Triangle
        triangleVertexArray->Bind();
        shaderProgram->UploadUniformFloat4("u_Color", glm::vec4(1.0f, 0.8f, trb, 1.0f));
        glDrawElements(GL_TRIANGLES, triangle.size(), GL_UNSIGNED_INT, nullptr);


        // Swap buffers to show our shape
        glfwSwapBuffers(window);

        // The window closes when ESC is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
    }

    shaderProgram->Unbind();

    glfwTerminate();

    return  EXIT_SUCCESS;

}
