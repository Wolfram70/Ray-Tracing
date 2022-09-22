#pragma once
#include "Walnut/Image.h"

#include <memory>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer() = default;

	void render();
	void onResize(uint32_t width, uint32_t height);

	std::shared_ptr<Walnut::Image> getFinalImage() const { return m_FinalImage; }

private:
	glm::vec4 perPixel(glm::vec2 coord);

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};

