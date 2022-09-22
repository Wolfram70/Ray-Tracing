#include "Sphere.h"

Sphere::Sphere(float r, glm::vec3 position)
{
	radius = r;
	Origin = position;
}

float Sphere::getRadius()
{
	return radius;
}

glm::vec3 Sphere::getOrigin()
{
	return Origin;
}
