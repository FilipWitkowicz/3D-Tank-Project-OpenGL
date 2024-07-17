#include <glm/glm.hpp>
#include "Structs.h"
#include "Ogniwo.h"
#include "vector"
class Gasienica {
private:
	Wheel W1, W2, W3, W4;
	Point2D P1, P2, P3;
	void CalculatePoints();
public:
	glm::mat4 Wheels[4];

	glm::mat4 Matrix;
	Ogniwo ogniwo;
	std::vector<Ogniwo> ogniwa;

	Gasienica(glm::mat4 Matrix);
	Gasienica(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	void Print(glm::mat4 Matrix);
	void Move(float tik);


};