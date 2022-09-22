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

SColResult Sphere::checkIntersect(Ray ray)
{
	SColResult result;

	glm::vec3 toRayOrigin = ray.Origin - Origin;
	float A = glm::dot(ray.Direction, ray.Direction);
	float B = 2 * glm::dot(ray.Direction, toRayOrigin);
	float C = glm::dot(toRayOrigin, toRayOrigin) - radius * radius;

	result.discriminant = B * B - 4 * A * C;
	float t = 0;

	if (result.discriminant < 0)
	{
		return result;
	}

	t = (-B - glm::sqrt(result.discriminant)) / (2 * A);

	result.hitPoint = ray.Origin + ray.Direction * t;
	result.normal = glm::normalize(result.hitPoint - Origin);

	Ray reflected;
	reflected.Origin = result.hitPoint;
	float d = 2.0f * glm::dot(glm::normalize(ray.Direction), result.normal);
	reflected.Direction = glm::normalize(ray.Direction) + d * result.normal;

	result.reflected = reflected;

	return result;
}
