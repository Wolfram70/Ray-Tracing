#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

struct SColResult
{
	float discriminant = 0;
	glm::vec3 hitPoint = glm::vec3(0.0f);
	glm::vec3 normal = glm::vec3(0.0f);
	Ray reflected;
};