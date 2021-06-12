#include "Akila/Akila.hpp"
#include "Akila/Test.hpp"
#include "glm/vec3.hpp"

int main() {
	Akila::hello();

	glm::vec3 pos{0, 5, 1};
	std::cout << pos.y << std::endl;

	std::cout << Akila::test() << std::endl;

	return 0;
}

