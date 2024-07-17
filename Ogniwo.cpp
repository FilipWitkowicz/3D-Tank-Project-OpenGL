#pragma once
#include "Ogniwo.h"
#include "math.h"
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;


Ogniwo::Ogniwo() {

}
Ogniwo::Ogniwo(Wheel W1, Wheel W2, Wheel W3, Wheel W4,
	Point2D P1, Point2D P2, Point2D P3)
{
	this->W1 = W1;
	this->W2 = W2;
	this->W3 = W3;
	this->W4 = W4;

	this->P1 = P1;
	this->P2 = P2;
	this->P3 = P3;


	FunctionIndex = 0;
	UpMaxAngle = 120;
	DownMaxAngle = 60;

	UpAngle = 0;
	DownAngle = 0;

}


void Ogniwo::Move(float tik) {

	if (tik < 0) DoFunction(-tik);
	else DoFunction(tik);

	if (tik < 0) {
		glm::mat4 M = glm::mat4(1.0f);
		M = glm::rotate(M, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		DrawMatrix = M * Matrix;
	}
	else {
		DrawMatrix = Matrix;
	}

}

void Ogniwo::DoFunction(float tik) {
	x = Matrix[3].x;
	y = Matrix[3].y;
	//Do some function
	if (FunctionIndex == 0) UpLine(tik);
	else if (FunctionIndex == 1) RightUpRotation(tik);
	else if (FunctionIndex == 2) RightLine(tik);
	else if (FunctionIndex == 3) RightDownRotation(tik);
	else if (FunctionIndex == 4) DownLine(tik);
	else if (FunctionIndex == 5) LeftDownRotation(tik);
	else if (FunctionIndex == 6) LeftLine(tik);
	else if (FunctionIndex == 7) LeftUpRotation(tik);
	FunctionIndex = FunctionIndex % 8;
	//cout << FunctionIndex << "\n";

}

float Ogniwo::GetAngle(float tik, float radius)
{
	float angle = 180 * tik / (3.1415926f * radius);
	return angle;
}

void Ogniwo::Print(glm::mat4 Matrix)
{
	for (int i = 0; i < 4; i++) {
		std::cout << Matrix[i].x << " " << Matrix[i].y << " " << Matrix[i].z << " " << Matrix[i].w << "\n";
	}
	cout << "\n";
}


void Ogniwo::UpLine(float tik)
{
	if (x + tik >= W1.Point.x) {
		FunctionIndex++;
		UpAngle = 0.0f;
		return;
	}

	Matrix = glm::translate(Matrix, glm::vec3(tik, 0.0f, 0.0f));


}

void Ogniwo::RightUpRotation(float tik)
{

	if (UpAngle > UpMaxAngle && tik > 0) {
		FunctionIndex++;
		//Correct(P2.x, P2.y, 240);
		return;
	}


	float step = GetAngle(tik, W1.Radius);

	glm::mat4 W1Matrix = glm::mat4(1.0f);
	W1Matrix[3].x = W1.Point.x;
	W1Matrix[3].y = W1.Point.y;

	glm::mat4 W1RMatrix = glm::mat4(1.0f);
	W1RMatrix[3].y = W1.Radius;

	W1Matrix = glm::rotate(W1Matrix, -glm::radians(UpAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	Matrix = W1Matrix * W1RMatrix;


	//cout << Matrix[3].x << " " << Matrix[3].y << endl;

	UpAngle += step;
}

void Ogniwo::RightLine(float tik)
{
	if (tik > 0 && y <= P3.y) {
		UpAngle = 0;

		FunctionIndex++;
		return;
	}
	Matrix = glm::translate(Matrix, glm::vec3(tik, 0.0f, 0.0f));
}

void Ogniwo::RightDownRotation(float tik)
{

	if (DownAngle > DownMaxAngle && tik > 0) {
		FunctionIndex++;
		Correct(W2.Point.x, W2.Point.y - W2.Radius, 180.0f);
		return;
	}
	float step = GetAngle(tik, W2.Radius);

	glm::mat4 W2Matrix = glm::mat4(1.0f);
	W2Matrix[3].x = W2.Point.x;
	W2Matrix[3].y = W2.Point.y;

	glm::mat4 W2RMatrix = glm::mat4(1.0f);
	W2RMatrix[3].y = W2.Radius;

	W2Matrix = glm::rotate(W2Matrix, -glm::radians(UpMaxAngle + DownAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	Matrix = W2Matrix * W2RMatrix;


	DownAngle += step;
}


void Ogniwo::DownLine(float tik)
{
	if (x + tik <= W3.Point.x) {
		FunctionIndex++;
		DownAngle = 0.0f;
		return;
	}

	Matrix = glm::translate(Matrix, glm::vec3(tik, 0.0f, 0.0f));
}

void Ogniwo::LeftDownRotation(float tik)
{

	if (DownAngle > DownMaxAngle && tik > 0) {
		FunctionIndex++;
		Correct(-P3.x, P3.y, 300); // <---------
		return;
	}
	float step = GetAngle(tik, W3.Radius);
	glm::mat4 W3Matrix = glm::mat4(1.0f);
	W3Matrix[3].x = W3.Point.x;
	W3Matrix[3].y = W3.Point.y;

	glm::mat4 W3RMatrix = glm::mat4(1.0f);
	W3RMatrix[3].y = -W3.Radius;

	W3Matrix = glm::rotate(W3Matrix, -glm::radians(DownAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	Matrix = W3Matrix * W3RMatrix;

	DownAngle += step;
}

void Ogniwo::LeftLine(float tik)
{
	if (tik > 0 && y >= P2.y) {
		UpAngle = 0;
		FunctionIndex++;
		return;
	}
	Matrix = glm::translate(Matrix, glm::vec3(-tik, 0.0f, 0.0f));
}

void Ogniwo::LeftUpRotation(float tik)
{
	if (UpAngle > UpMaxAngle && tik > 0) {
		FunctionIndex++;
		Correct(W4.Point.x, W4.Point.y + W4.Radius, 0.0f);
		return;
	}


	float step = GetAngle(tik, W4.Radius);

	glm::mat4 W4Matrix = glm::mat4(1.0f);
	W4Matrix[3].x = W4.Point.x;
	W4Matrix[3].y = W4.Point.y;

	glm::mat4 W4RMatrix = glm::mat4(1.0f);
	W4RMatrix[3].y = W4.Radius;

	W4Matrix = glm::rotate(W4Matrix, -glm::radians(-UpMaxAngle + UpAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	Matrix = W4Matrix * W4RMatrix;


	//cout << Matrix[3].x << " " << Matrix[3].y << endl;

	UpAngle += step;
}

void Ogniwo::Correct(float x, float y, float angle) {
	Matrix = glm::mat4(1.0f);
	Matrix = glm::rotate(Matrix, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	Matrix[3].x = x;
	Matrix[3].y = y;
	UpAngle = 0;
	DownAngle = 0;

}