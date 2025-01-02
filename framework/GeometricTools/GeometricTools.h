#ifndef __GEOMETRICTOOLS_H_
#define __GEOMETRICTOOLS_H_

#include <array>
#include <iostream>
#include <iomanip>
#include <vector>



// Max divisions of the grid
constexpr int maxGridSize = 10;


namespace GeometricTools {

    // A standard 2D triangle
    constexpr std::array<float, 3*2> UnitTriangle2D = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f, 0.5f
    };


    constexpr std::array<float, 4*2> UnitSquare2D = {

        // These indices make the following triangles:

        // 2-------3
        // |  \    |
        // |    \  |
        // 0-------1

        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f
    };


    // # of coordinates (x,y,z) *
    // # of corners *
    // # of sides
    constexpr std::array<float, 3*4*2> UnitCube = {

        // Front
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,

        // Back
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f

    };


    // Normals
    // # of coordinates (x,y,z) *
    // # of corners *
    // 2 for normals *
    // # of sides
    constexpr std::array<float, 3 * 4 * 2 * 6> UnitCube3D24WNormals = {

        // Front
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        // Back
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        // Left
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        // Right
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        // Top
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        // Bottom
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f
    };





    // These indices make the following triangles:


    //     4-------6
    //   /       /
    // 0-------2
    // |  \    |
    // |    \  |
    // 1-------3
    //   \       \
    //     5-------7


    constexpr std::array<GLuint, 6*6> UnitCubeTopologyTriangles = {

        // Front
        0,1,3,
        0,2,3,

        // Back
        5,4,6,
        5,7,6,


        // Left
        1,0,4,
        1,5,4,


        // Right
        2,3,7,
        2,6,7,


        // Top
        4,0,2,
        4,6,2,


        // Bottom
        1,5,7,
        1,3,7

    };

    constexpr std::array<unsigned int, 6 * 3 * 2> UnitCube3DTopologyTriangles24 = {
                
        // Front
        0,1,2,
        2,1,3,
    
        // Back
        4,5,6,
        6,5,7,
    
        // Left
        8,9,10,
        10,9,11,
    
        // Right
        12,13,14,
        14,13,15,
    
        // Top
        16,17,18,
        18,17,19,
    
        // Bottom
        20,21,22,
        22,21,23
    };


    // x and y: How many divisions should be in the grid
    inline std::vector<float> UnitGridGeometry2D(const int x, const int y) {

        std::vector<float> arr{};

        if (x > maxGridSize || y > maxGridSize) {
            std::cerr << "\nGrid can max have " << maxGridSize << " divisions!" << std::endl;
            return arr;
        }

        // j is how many times the squares are to be drawn aka how divided the grid is
        int j = 0;

        // The current coordinates on the grid
        float posx = 0.0f;
        float posy = 0.0f;


        // Adds an optional small gap between the squares
        float border = 0.999f;

        while (j < x*y) {

            // The order of numbers are posx, posy, texx, texy

            for (int i = 0; i < UnitSquare2D.size(); ++i) {

                // Divide the original size on x / y and shift to the correct position
                float newCoor = UnitSquare2D[i];

                // The x coordinates
                if (i % 2 == 0) {
                    newCoor =
                            // The width of a single square
                            newCoor / x * border

                            // Each square is to be placed posx/x to the right of the previous one
                            + posx / x

                            // The middle of the first square is now in the middle of the screen,
                            // so this sets the left edge of the first square at the center

                            + 0.5f / x

                            // Move the entire grid backwards
                            - 0.5f;
                }

                // The y coordinates
                else {
                    newCoor =
                            newCoor / y * border

                            + posy / y

                            + 0.5f / y

                            - 0.5f;
                }

                arr.push_back(newCoor);

            }
            j++;

            // Skips to the next row when the entire row of squares is filled
            posx++;
            if (posx >= x) {
                posx = 0.0f;
                posy++;
            }
        }

        return arr;
    }

    inline std::vector<GLuint> UnitGridTopologyTriangles(const int x, const int y) {

        std::vector<GLuint> square_indices{};

        for (int i = 0; i < 6 * x * y; i += 4) {
            square_indices.push_back(i);
            square_indices.push_back(i + 1);
            square_indices.push_back(i + 3);
            square_indices.push_back(i);
            square_indices.push_back(i + 2);
            square_indices.push_back(i + 3);
        }

        return square_indices;
    }

}



#endif // __GEOMETRICTOOLS_H_
