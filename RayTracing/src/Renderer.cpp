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

void Renderer::render(const Camera& camera)
{
	Ray ray;
	ray.Origin = camera.GetPosition();

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

			glm::vec4 color = traceRay(ray);
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

glm::vec4 Renderer::traceRay(const Ray& ray)
{
	//light
	glm::vec3 lightDirection = glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f));
	//glm::vec3 lightDirection = ray.Direction;
	
	SColResult ballResult = ball.checkIntersect(ray);
	if (ballResult.collide)
	{
		glm::vec3 ballSphereColor(1, 0, 1);

		float d = glm::max(glm::dot(ballResult.normal, -lightDirection), 0.0f);

		ballSphereColor = ballSphereColor * d;

		return glm::vec4(ballSphereColor, 1.0f);
	}

	SColResult mirrorResult = mirror.checkIntersect(ray);
	if (mirrorResult.collide)
	{
		glm::vec3 mirrorSphereColor(0, 1, 0);

		float d = glm::max(glm::dot(mirrorResult.normal, -lightDirection), 0.0f);

		mirrorSphereColor = mirrorSphereColor * d;

		ballResult = ball.checkIntersect(mirrorResult.reflected);
		if (ballResult.collide)
		{
			glm::vec3 ballSphereColor(1, 0, 1);

			float d = glm::max(glm::dot(ballResult.normal, -lightDirection), 0.0f);

			mirrorSphereColor += ballSphereColor * d;
		}

		return glm::vec4(mirrorSphereColor, 1.0f);
	}
	
	return glm::vec4(0, 0, 0, 1);
}
