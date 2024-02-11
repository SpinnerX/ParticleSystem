#include "GameLayer.h"
#include <GameEngine/Core/GameEngine.h>

class Sandbox : public RendererEngine::Application{
public:
	Sandbox(RendererEngine::ApplicationCommandLineArgs args) : Application("Game", args){
		pushLayer(new GameLayer());
	}

	~Sandbox(){}
};

RendererEngine::Application* RendererEngine::CreateApplication(ApplicationCommandLineArgs args){
	return new Sandbox(args);
}
