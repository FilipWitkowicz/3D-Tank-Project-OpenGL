#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Point2D {
	float x;
	float y;
};
struct Wheel {
	float Radius;
	Point2D Point;
};
struct CustomObject {
	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec4 > normals;
};
