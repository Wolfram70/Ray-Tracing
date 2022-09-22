#pragma once
#include <glm/glm.hpp>

class Sphere
{
public:
	Sphere(float r, glm::vec3 position);
	float getRadius();
	glm::vec3 getOrigin();

private:
	glm::vec3 Origin = { 0.0f, 0.0f, 0.0f };
	float radius;
};

