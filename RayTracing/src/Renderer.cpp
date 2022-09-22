#include "Renderer.h"

#include "Walnut/Random.h"

namespace Utils 
{
	static uint32_t convertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;

		return result;
	}
}

void Renderer::render()
{
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth() , (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f;

			glm::vec4 color = perPixel(coord);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::convertToRGBA(color);
		}
	}
	
	m_FinalImage->SetData(m_ImageData);
}

void Renderer::onResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		//no resize necessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

glm::vec4 Renderer::perPixel(glm::vec2 coord)
{
	//light
	glm::vec3 lightDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

	//camera
	glm::vec3 camOrigin = { 0.0f, 0.0f, 3.0f };
	glm::vec3 rayDirection = { coord, 0.0f }; rayDirection -= camOrigin; //can normalise for unit length
	
	//sphere stuff
	glm::vec3 sphereOrigin = { 0.5f, 0.0f, 0.0f };
	glm::vec3 sphereToCam = camOrigin - sphereOrigin;
	float radius = 0.5f;
	float A = glm::dot(rayDirection, rayDirection);
	float B = 2 * glm::dot(rayDirection, sphereToCam);
	float C = glm::dot(sphereToCam, sphereToCam) - radius * radius;

	//calculation
	float discriminant = B * B - 4 * A * C;

	if (discriminant >= 0)
	{
		//float t0 = (-B + glm::sqrt(discriminant)) / (2 * A);
		float t1 = (-B - glm::sqrt(discriminant)) / (2 * A);
		
		glm::vec3 hitPoint = camOrigin + rayDirection * t1;
		glm::vec3 normal = glm::normalize(hitPoint - sphereOrigin);
		glm::vec3 sphereColor(1, 0, 1);

		float d = glm::max(glm::dot(normal, -lightDirection), 0.0f);

		sphereColor = sphereColor * d;

		return glm::vec4(sphereColor, 1.0f);
	}
	
	return glm::vec4(0, 0, 0, 1);
}
