#include "Pocisk.h"
#include <glm/glm.hpp>
#include <iostream>

using namespace std;

Pocisk::Pocisk()
{
	Active = false;
	Matrix = glm::mat4(1.0f);
	LifeTimeMax = 10.0f;
	LifeTime = LifeTimeMax;
	Speed = 0.5f;
}

Pocisk::Pocisk(float LifeTimeMax, float Speed, glm::mat4 Matrix = glm::mat4(1.0f))
{
	Active = false;
	this->LifeTimeMax = LifeTimeMax;
	this->Matrix = Matrix;
	this->LifeTime = LifeTimeMax;
	this->Speed = Speed;
}
