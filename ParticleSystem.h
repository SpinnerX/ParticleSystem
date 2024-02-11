#pragma once
// #include <GameEngine/Core/GameEngine.h>
#include <GameEngine/Core/Timestep.h>
#include <GameEngine/Renderer/OrthographicCamera.h>
#include <vector>

struct particleProps{
	glm::vec2 pos;
	glm::vec2 velocity, velocityVariation;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifetime = 1.0f;
};

class particleSystem{
public:
	particleSystem(uint32_t maxParticles=100000);

	void onUpdate(RendererEngine::Timestep ts);

	void onRender(RendererEngine::OrthographicCamera& camera);

	void emit(const particleProps& props);


private:
	struct particle{
		glm::vec2 pos;
		glm::vec2 velocity;
		glm::vec4 colorBegin, colorEnd;
		float rotation = 0.0f;
		float lifetime = 1.0f;
		float lifeRemaining = 0.0f;
		float sizeBegin, sizeEnd;

		bool isActive = false;
	};

	std::vector<particle> _particlesPool;
	uint32_t _poolIndex;
};