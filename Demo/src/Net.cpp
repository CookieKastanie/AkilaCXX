#include "Demo/Net.hpp"

Net::Point::Point(float x, float y, float z): position{x, y, z}, prevPosition{x, y, z}, isLocked{false} {}

Net::Stick::Stick(Point &a, Point &b) : pointA{a}, pointB{b} {
    length = glm::length(a.position - b.position);
    halfLength = length / 2.;
}

Net::Net() : gpuDatas{
    {},
    {},
    {3, 0, Akila::Buffer::Usage::DYNAMIC},
    {3, 1, Akila::Buffer::Usage::DYNAMIC},
    {},
    false
} {

}

void Net::update() {
    for(Point &p : points) {
        if(!p.isLocked) {
            // conservation du mouvement
            const glm::vec3 lastPos = p.position;
            p.position = p.position + (p.position - p.prevPosition);
            p.prevPosition = lastPos;

            //foreachPoint(p);
            p.position.y -= Akila::Time::fixedDelta;
        }
    }

    // contraintes
    for(int i = 0; i < 20; ++i) {
        for(Stick &stick : sticks) {
            const glm::vec3 &&stickCenter = stick.pointA.position * glm::vec3(0.5) + stick.pointB.position * glm::vec3(0.5);
            const glm::vec3 &&stickDir = glm::normalize(stick.pointA.position - stick.pointB.position) * glm::vec3(stick.halfLength);

            if(!stick.pointA.isLocked) stick.pointA.position = stickCenter + stickDir;
            if(!stick.pointB.isLocked) stick.pointB.position = stickCenter - stickDir;
        }
    }

    if(gpuDatas.swap) {
        gpuDatas.vboA.setData(gpuDatas.positionsBuffer);
    } else {
        gpuDatas.vboB.setData(gpuDatas.positionsBuffer);
    }
    gpuDatas.swap = !gpuDatas.swap;
}

void Net::draw() {
    auto &&shader = Akila::Core::resourcePool->getShader("netShader");

    int i = 0;
    for(auto &p : points) {
        gpuDatas.positionsBuffer[i++] = p.position;
    }

    /*float mix = Akila::Time::mix;
    if(gpuDatas.swap) {
        gpuDatas.vboA.setData(gpuDatas.positionsBuffer);
        std::cout << "A\n";
    } else {
        gpuDatas.vboB.setData(gpuDatas.positionsBuffer);
        //mix = 1. - Akila::Time::mix;
        std::cout << "B\n";
    }
    gpuDatas.swap = !gpuDatas.swap;*/

    shader->bind();

    float mix = Akila::Time::mix;
    if(!gpuDatas.swap) {
        mix = 1. - Akila::Time::mix;
    }

    shader->send("t", mix);
    gpuDatas.vao.draw();
}

void Net::fillGrid(Net &net, int size) {
    for(int y = 0; y < size; ++y) {
        for(int x = 0; x < size; ++x) {
            //Point p = Point(x * 0.15, -y * 0.15, y * 0.1);
            Point p = Point(y * 0.05, -y * 0.15 + 1., x * 0.15);
            if(x == 0 && y == 0) p.isLocked = true;
            //else if(x == (int)(size / 2) && y == 0) p.isLocked = true;
            else if(x == size - 1 && y == 0) p.isLocked = true;

            net.points.push_back(p);
        }
    }

    std::vector<unsigned short> indices;
    std::function<int(int, int)> coordToIndex = [size](int x, int y) -> int {
        return size * y + x;
    };

    for(int y = 0; y < size; ++y) {
        for(int x = 0; x < size; ++x) {
            if(x < size - 1) {
                net.sticks.push_back({
                    net.points[coordToIndex(x, y)],
                    net.points[coordToIndex(x + 1, y)]
                                     });

                indices.push_back(coordToIndex(x, y));
                indices.push_back(coordToIndex(x + 1, y));
            }

            if(y < size - 1) {
                net.sticks.push_back({
                    net.points[coordToIndex(x, y)],
                    net.points[coordToIndex(x, y + 1)]
                                     });

                indices.push_back(coordToIndex(x, y));
                indices.push_back(coordToIndex(x, y + 1));
            }
        }
    }

    net.gpuDatas.positionsBuffer.resize(net.points.size());
    int i = 0;
    for(auto &p : net.points) {
        net.gpuDatas.positionsBuffer[i++] = p.position;
    }
    net.gpuDatas.vboA.setData(net.gpuDatas.positionsBuffer);
    net.gpuDatas.vboB.setData(net.gpuDatas.positionsBuffer);

    net.gpuDatas.ibo.setData(indices);

    net.gpuDatas.vao.bind();
    net.gpuDatas.vao.registerIBO(&net.gpuDatas.ibo);
    net.gpuDatas.vao.registerVBO(&net.gpuDatas.vboA);
    net.gpuDatas.vao.registerVBO(&net.gpuDatas.vboB);
    net.gpuDatas.vao.unbind();
    net.gpuDatas.vao.setDrawMode(Akila::VAO::Mode::LINES);
}
