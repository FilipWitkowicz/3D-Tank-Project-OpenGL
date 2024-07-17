#pragma once
#include <glm/glm.hpp>
#include "Structs.h"
class Ogniwo {
	Wheel W1, W2, W3, W4;
	Point2D P1, P2, P3;
	float UpAngle, DownAngle;
	float UpMaxAngle, DownMaxAngle;
	int FunctionIndex;
	int Functions[8];
	float x, y;


public:
	glm::mat4 Matrix; //matrix used for calculations
	glm::mat4 DrawMatrix; //final matrix used for drawing
	Ogniwo();
	Ogniwo(Wheel W1, Wheel W2, Wheel W3, Wheel W4,
		Point2D P1, Point2D P2, Point2D P3);
	void Move(float tik);
	void DoFunction(float tik);
	float GetAngle(float tik, float radius);
	void Print(glm::mat4 Matrix);
	void UpLine(float tik);
	void RightUpRotation(float tik);
	void RightLine(float tik);
	void RightDownRotation(float tik);
	void DownLine(float tik);
	void LeftDownRotation(float tik);
	void LeftLine(float tik);
	void LeftUpRotation(float tik);
	void Correct(float x, float y, float angle);
};