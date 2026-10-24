#include "../../include/game.h"
#include "../../include/graphics/models/spriteRenderer.h"
#include "../../include/memory/ResourceManager.h"

SpriteRenderer* Renderer;

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_ACTIVE), Keys(), width(width), height(height){

}

Game::~Game(){
	delete Renderer;
}

void Game::init(){
	// load shaders
	ResourceManager::createShader("sprite", false);
	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 
							static_cast<float>(height), 0.0f, -1.0f, 1.0f);

	ResourceManager::getShader("sprite").activate();
	ResourceManager::getShader("sprite").setInt("image", 0);
	ResourceManager::getShader("sprite").setMat4("projection", projection);
	
	Renderer = new SpriteRenderer(ResourceManager::getShader("shader"));
	ResourceManager::createTexture("awesomeface.png");
}

void Game::processInput(float dt)
{
}

void Game::update(float dt)
{
}

void Game::render(){
	Renderer->DrawSprite(ResourceManager::getTexure("awesomeface.png"),
		glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
