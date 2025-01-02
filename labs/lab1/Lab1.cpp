#include "shaders.h"
#include "GLFWApplication/GLFWApplication.h"
#include "GeometricTools/GeometricTools.h"
#include "Lab1.h"

using namespace GeometricTools;

const int vertex_amount = 3;

GLuint createVAO(float object[], int object_size, int axes);
GLuint createEBO(GLuint object[], int object_size);
GLuint CompileShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
void CleanVAO(GLuint &vao);


int lab1_App::run() {


    auto shaderProgram = CompileShader(vertex, fragment);


    GLuint vertexArrayId[vertex_amount];
    GLuint ebo[2];



    //////////////// Square //////////////////

    auto square = UnitSquare2D;

    auto square_obj = createVAO(square.data(), sizeof(square), 2);

    vertexArrayId[0] = square_obj;

    auto square_indices = UnitGridTopologyTriangles(1, 1);

    auto square_ebo = createEBO(square_indices.data(), square_indices.size());

    ebo[0] = square_ebo;

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, square_indices.size()*2, square_indices.data(), GL_STATIC_DRAW);




    //////////////// Grid //////////////////


    int x = 4;
    int y = 3;

    auto grid = UnitGridGeometry2D(x, y);

    auto grid_obj = createVAO(grid.data(), grid.size()*4, 2);

    vertexArrayId[2] = grid_obj;



    auto grid_indices = UnitGridTopologyTriangles(x, y);


    auto grid_ebo = createEBO(grid_indices.data(), sizeof(grid_indices));

    ebo[1] = grid_ebo;

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, grid_indices.size()*x*y, grid_indices.data(), GL_STATIC_DRAW);




    //////////////// Triangle //////////////////

    auto triangle = UnitTriangle2D;

    auto triangle_obj = createVAO(triangle.data(), sizeof(triangle), 2);
    vertexArrayId[1] = triangle_obj;


    double currentTime;
    glfwSetTime(0.0);


    // Application loop code (SECTION 5)

    std::cout << "Hello World!" << std::endl;
    while (!glfwWindowShouldClose(window)) {

        // Poll for events (like input)
        glfwPollEvents();

        // Time management for alternate flag
        currentTime = glfwGetTime();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);


        //    This step fetches the location of the uniform variable "u_Color" within the shader program.
        auto vertexColorLocation = glGetUniformLocation(shaderProgram, "u_Color");
        auto trb = (sin(currentTime) / 2.0f) + 0.5f;
        auto sqg = (sin(currentTime) / 5.0f) + 0.5f;
        auto bgr = (sin(currentTime) / 5.0f) + 0.5f;


        // Background color
        glClearColor(bgr, 0.2f, 0.6f, 1.0f);


        // Square
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayId[0]);
        glUniform4f(vertexColorLocation, 1.0f, sqg, 0.6f, 1.0f);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
        glDrawElements(GL_TRIANGLES, square.size(), GL_UNSIGNED_INT, nullptr);



        // Grid
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayId[2]);
        glUniform4f(vertexColorLocation, bgr, 0.0f, trb, 1.0f);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
        glDrawElements(GL_TRIANGLES, grid.size(), GL_UNSIGNED_INT, nullptr);


        // Triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayId[1]);
        glUniform4f(vertexColorLocation, 1.0f, 0.8f, trb, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, triangle.size());


        // Swap buffers to show our shape
        glfwSwapBuffers(window);

        // The window closes when ESC is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
    }


    // Termination code (SECTION 6)
    glDisableVertexAttribArray(0);
    CleanVAO(*vertexArrayId);


    glfwTerminate();

    return  EXIT_SUCCESS;

}




// Create a vertex array and a vertex buffer out of a given object
GLuint createVAO(float object[], int object_size, int axes) {

    // Create a vertex array
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);

    // Create a vertex buffer
    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);


    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

    // Populate the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, object_size, object, GL_STATIC_DRAW);

    // Set the layout of the bound buffer
    glVertexAttribPointer(0, axes, GL_FLOAT, GL_FALSE, sizeof(float)*axes, nullptr);

    glEnableVertexAttribArray(0);

    return vertexArrayId;
}


// Create an element buffer object out of a given object
GLuint createEBO(GLuint object[], int object_size) {

    // Create an element buffer object
    GLuint ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object_size, object, GL_STATIC_DRAW);


    return ebo;
}



// -----------------------------------------------------------------------------
// COMPILE SHADER
// -----------------------------------------------------------------------------
GLuint CompileShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc) {

    auto vertexSrc = vertexShaderSrc.c_str();
    auto fragmentSrc = fragmentShaderSrc.c_str();

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);

    auto shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}



// -----------------------------------------------------------------------------
// Clean VAO
// -----------------------------------------------------------------------------
void CleanVAO(GLuint &vao) {

    /* === CLEANUP ===
    * Freeing resources after they are no longer needed is good practice.
    * We delete our buffers and terminate GLFW.
    */

    glBindVertexArray(vao);
    GLint maxVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
    for(GLint i = 0; i < maxVertexAttribs; i++)
    {
        glDisableVertexAttribArray(i);
    }
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    vao = 0;
}