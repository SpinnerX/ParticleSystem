#pragma once
#include <GameEngine/Core/Application.h>
#include <GameEngine/Core/Layer.h>
#include <GameEngine/Renderer/Texture.h>
#include <GameEngine/Renderer/SubTexture2D.h>
#include <GameEngine/Renderer/OrthographicCameraController.h>
#include <GameEngine/Renderer/RenderCommand.h>
#include "ParticleSystem.h"
#include <GameEngine/Renderer/Renderer2D.h>
using namespace RendererEngine;

static const uint32_t mapWidth = 24;
static const char* _mapTiles =
"GGGGGGGGGGGGGGGGGGGGGGGG"
"GGGGGGGGGGGGGGGGGGGGGGGG"
"GGGGGGGDDDDDDDGGGGWWWGGG"
"GGGGGGDDDDDDDDDDGGWWWGGG"
"GGGGGDDDDDDDDDDDDGWWGGGG"
"GGGGGDDDDGGDDDDDDDGGGGGG"
"GGGGGDDDDGGDDDDDDDGGGGGG"
"GGGGGDDDDDDDDDDDDDGGGGGG"
"GGGGGDDDDDDDDDDDDDGGGGGG"
"GGGGGDDDDDDDDDDDDDGGGGGG"
"GGGGGGDDDDDDDDDDDDGGGGGG"
"GGGGGGGDDDDDDDDDDGGGGWWW"
"WWWGGGGGDDDDDDDDGGGGWWWW"
"GGGGGGGGGDDDDDDDGGGGGGGG"
"GGGGGGGGGGGGGGGGGGGGGGGG"
"GGGGGGGGGGGGGGGGGGGGGGGG";

class GameLayer : public Layer{
public:
	GameLayer() : Layer("Game Layer"), _cameraController(1280.0f / 720.0f){

    }

	virtual ~GameLayer() = default;

	virtual void onAttach() override{
        // texture1 = Texture2D::Create("assets/Checkerboard.png");
        spriteSheets = Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");
        stairsTexture = SubTexture2D::createFromCoords(spriteSheets, {0, 11}, {128, 128});
        treeTexture = SubTexture2D::createFromCoords(spriteSheets, {2, 1}, {128, 128}, {1, 2});
        grassTexture = SubTexture2D::createFromCoords(spriteSheets, {1, 5}, {128, 128});

        _particle.colorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
        _particle.colorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
        _particle.sizeBegin = 0.5f, _particle.sizeVariation = 0.3f, _particle.sizeEnd = 0.0f;
        _particle.lifetime = 1.0f;
        _particle.velocity = { 0.0f, 0.0f };
        _particle.velocityVariation = { 3.0f, 1.0f };
        _particle.pos = { 0.0f, 0.0f };

        _cameraController.setZoomLevel(5.0f);
		
	}

	virtual void onDetach() override{}

	virtual void onImguiRender() override{
        ImGui::Begin("Settings");
        auto stats = RendererEngine::Renderer2D::getStats();
        ImGui::Text("Renderer2D Stats");
        ImGui::Text("Draw Calls %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());

        ImGui::End();

    }

	virtual void onUpdate(Timestep ts) override{
        Renderer2D::resetStats();
        RendererCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RendererCommand::clear();

        if(InputPoll::isMouseButtonPressed(RENDER_MOUSE_BUTTON_LEFT)){
            glm::vec2 mousePos = InputPoll::getMousePosition();
            float x = mousePos.x;
            float y = mousePos.y;
            auto w = Application::Get().GetWindow().getWidth();
            auto h = Application::Get().GetWindow().getHeight();

            auto bounds = _cameraController.getBounds();
            auto pos = _cameraController.getCamera().getPosition();
            x = (x/w) * bounds.getWidth() - bounds.getWidth() * 0.5f;
            y = bounds.getHeight() * 0.5f - (y / h) * bounds.getHeight();

            _particle.pos = {x + pos.x, y+pos.x};

            for(int i = 0; i < 5; i++)
                _particleSystem.emit(_particle);
        }


        _particleSystem.onUpdate(ts);
        _particleSystem.onRender(_cameraController.getCamera());
        Renderer2D::beginScene(_cameraController.getCamera());
        for(uint32_t y = 0; y < _mapHeight; y++){
            for(uint32_t x = 0; x < _mapWidth; x++){
                
                char tileType = _mapTiles[x + y * _mapWidth];
                Ref<SubTexture2D> texture = SubTexture2D::createFromCoords(spriteSheets, {11, 1}, {128, 128});

                if(textureMap.contains(tileType)){
                    texture = textureMap[tileType];
                }

                Renderer2D::drawQuad({ x - _mapWidth / 2.0f, _mapHeight - y-_mapHeight/2.0f, 0.5f}, {1.0f, 1.0f}, texture);
            }
        }

        Renderer2D::drawQuad({0.0f, 0.0f, 0.5f}, {1.0f, 1.0f}, stairsTexture); // stairs texture shape
        Renderer2D::drawQuad({1.0f, 0.0f, 0.5f}, {1.0f, 1.0f}, treeTexture); // barrel texture shape
        Renderer2D::drawQuad({-1.0f, 0.0f, 0.5f}, {1.0f, 2.0f}, grassTexture); //  tree texture shape

        Renderer2D::endScene();
    }

	virtual void onEvent(Event& e) override{
        EventDispatcher dispatch(e);
        dispatch.Dispatch<WindowCloseEvent>(bind_function(this, &GameLayer::onWindowClose));
		_cameraController.onEvent(e);
    }

private:
    bool onWindowClose(WindowCloseEvent e){
        Application::Get().close();
        return false;
    }

private:
    OrthographicCameraController _cameraController;
    Ref<Texture2D> spriteSheets;
    Ref<SubTexture2D> stairsTexture;
    Ref<SubTexture2D> treeTexture;
    Ref<SubTexture2D> grassTexture;

    glm::vec4 color = {0.2f, 0.3f, 0.8f, 1.0f};

    particleSystem _particleSystem;
	particleProps _particle;
	uint32_t _mapWidth;
	uint32_t _mapHeight;
	std::unordered_map<char, Ref<SubTexture2D> > textureMap;
};
