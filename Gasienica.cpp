#pragma once
#include "Gasienica.h"
#include "Ogniwo.h"
#include "math.h"
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
void Gasienica::CalculatePoints()
{
	float alfa = 30.0f;
	float h = 0.24f;
	W1.Point.x = 3.1f;
	W1.Point.y = 0.55f;
	W1.Radius = 0.3f;
	P1.x = W1.Point.x;
	P1.y = W1.Point.y + W1.Radius;
	P2.x = W1.Point.x + W1.Radius * cos(glm::radians(alfa));
	P2.y = W1.Point.y - W1.Radius * sin(glm::radians(alfa));

	W2.Point.x = W1.Point.x - (h / tan(glm::radians(90.0f - alfa)));
	W2.Point.y = W1.Point.y - h;
	W2.Radius = 0.3f;
	P3.x = W2.Point.x + W2.Radius * cos(glm::radians(alfa));
	P3.y = W2.Point.y - W2.Radius * sin(glm::radians(alfa));

	W3.Point.x = -W2.Point.x;
	W3.Point.y = W2.Point.y;
	W3.Radius = W2.Radius;

	W4.Point.x = -W1.Point.x;
	W4.Point.y = W1.Point.y;
	W4.Radius = W1.Radius;


}

Gasienica::Gasienica(float x, float y, float z)
{
	CalculatePoints();

	Matrix = glm::mat4(
		glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
		glm::vec4(x, y, z, 1.0f)
	);
	Wheels[0] = glm::translate(Matrix, glm::vec3(W1.Point.x, W1.Point.y, 0.0f));
	Wheels[1] = glm::translate(Matrix, glm::vec3(W2.Point.x, W2.Point.y, 0.0f));
	Wheels[2] = glm::translate(Matrix, glm::vec3(W3.Point.x, W3.Point.y, 0.0f));
	Wheels[3] = glm::translate(Matrix, glm::vec3(W4.Point.x, W4.Point.y, 0.0f));



	for (int i = 0; i < 62; i++) {
		Ogniwo _ogniwo = Ogniwo(W1, W2, W3, W4, P1, P2, P3);
		_ogniwo.Matrix = glm::translate(this->Matrix, glm::vec3(0.0f, P1.y, 0.0f));
		for (int j = 0; j < i; j++)
			_ogniwo.Move(0.3f);
		ogniwa.push_back(_ogniwo);

	}
	for (int i = 0; i < ogniwa.size(); i++) {
		for (int j = 0; j < 100; j++) {
			ogniwa[i].Move(0.04);
		}
	}
}

Gasienica::Gasienica(glm::mat4 Matrix)
{

	CalculatePoints();

	this->Matrix = Matrix;
	Ogniwo _ogniwo(W1, W2, W3, W4, P1, P2, P3);
	ogniwo = _ogniwo;
}




void Gasienica::Print(glm::mat4 Matrix)
{
	for (int i = 0; i < 4; i++) {
		std::cout << Matrix[i].x << " " << Matrix[i].y << " " << Matrix[i].z << " " << Matrix[i].w << "\n";
	}
}

void Gasienica::Move(float tik)
{
	for (int i = 0; i < ogniwa.size(); i++) {
		ogniwa[i].Move(tik);
	}
	//ogniwo.Move(tik);
}