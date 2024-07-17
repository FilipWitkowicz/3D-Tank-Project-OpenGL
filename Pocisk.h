#pragma once
#include <glm/glm.hpp>

class Pocisk {
public:
	glm::mat4 Matrix;
	bool Active;
	float LifeTimeMax, LifeTime, Speed;
	Pocisk();
	Pocisk(float LifeTimeMax, float Speed, glm::mat4 Matrix);

};