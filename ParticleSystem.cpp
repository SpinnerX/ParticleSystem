#include "ParticleSystem.h"
#include <random>

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIEMENTAL
#include <glm/gtx/compatibility.hpp>
#include <GameEngine/Renderer/Renderer2D.h>

class Random{
public:
	static void init(){
		randomEngine.seed(std::random_device()());
	}

	static float Float(){
		return (float)distribution(randomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}
private:
	static std::mt19937 randomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> distribution;
};


std::mt19937 Random::randomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::distribution;

particleSystem::particleSystem(uint32_t maxParticles) : _poolIndex(maxParticles - 1){
	_particlesPool.resize(maxParticles);
}

void particleSystem::onUpdate(RendererEngine::Timestep ts){
	for(auto& particle : _particlesPool){
		if(!particle.isActive) continue;

		if(particle.lifeRemaining <= 0.0f){
			particle.isActive = false;
			continue;
		}

		particle.lifeRemaining -= ts;
		particle.pos += particle.velocity * (float)ts;
		particle.rotation += 0.01 * ts;
	}
}

void particleSystem::onRender(RendererEngine::OrthographicCamera& camera){
	RendererEngine::Renderer2D::beginScene(camera);
	for(auto& particle : _particlesPool){


		if(!particle.isActive) continue;

		float life = particle.lifeRemaining / particle.lifetime;
		glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);

		float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);
		
		glm::vec3 position = {particle.pos.x, particle.pos.y, 0.2f};
		RendererEngine::Renderer2D::drawRotatedQuad(position, {size, size}, particle.rotation, color);
	}	
	RendererEngine::Renderer2D::endScene();
}

void particleSystem::emit(const particleProps& props){
	particle& p = _particlesPool[_poolIndex];
	p.isActive = true;
	p.pos = props.pos;
	p.rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	p.velocity = props.velocity;
	p.velocity.x += props.velocityVariation.x * (Random::Float() - 0.5f);
	p.velocity.y += props.velocityVariation.y * (Random::Float() - 0.5f);

	// Color
	p.colorBegin = props.colorBegin;
	p.colorEnd = props.colorEnd;

	p.lifetime = props.lifetime;
	p.lifeRemaining = props.lifetime;
	p.sizeBegin = props.sizeBegin + props.sizeVariation * (Random::Float() - 0.5f);
	p.sizeEnd = props.sizeEnd;

	_poolIndex = --_poolIndex % _particlesPool.size();
}