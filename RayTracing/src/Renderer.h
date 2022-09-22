#pragma once
#include "Walnut/Image.h"

#include "Camera.h"
#include "Ray.h"
#include "Sphere.h"
#include "SColResult.h"

#include <memory>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer() = default;

	void render(const Camera& camera);
	void onResize(uint32_t width, uint32_t height);

	std::shared_ptr<Walnut::Image> getFinalImage() const { return m_FinalImage; }

private:
	glm::vec4 traceRay(const Ray& ray);

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

	//temporary
	Sphere ball = Sphere(0.5f, { -4.0f, 0.0f, 0.0f });
	Sphere mirror = Sphere(0.5f, { 1.0f, 0.0f, 0.0f });
};

