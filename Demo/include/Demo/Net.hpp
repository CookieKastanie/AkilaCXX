#pragma once

#include "glm/glm.hpp"
#include <vector>

#include "Akila/core/Core.hpp"
#include "Akila/graphics/gl/Buffer.hpp"
#include "Akila/graphics/gl/VAO.hpp"

class Net {
private:
    struct GPUDatas {
        Akila::VAO vao;
        Akila::IBO ibo;
        Akila::VBO vboA;
        Akila::VBO vboB;
        std::vector<glm::vec3> positionsBuffer;
        bool swap;
    };

public:
    struct Point {
        glm::vec3 position;
        glm::vec3 prevPosition;
        bool isLocked;

        Point(float x = 0, float y = 0, float z = 0);
    };

    struct Stick {
        Point &pointA;
        Point &pointB;
        float length;
        float halfLength;

        Stick(Point &a, Point &b);
    };

private:
    std::vector<Point> points;
    std::vector<Stick> sticks;

    GPUDatas gpuDatas;

public:
    Net();
    void update();
    void draw();

    static void fillGrid(Net &net, int size = 30);
};
