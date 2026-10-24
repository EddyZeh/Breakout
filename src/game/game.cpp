#include "../../include/game/game.h"
#include "../../include/graphics/models/spriteRenderer.h"
#include "../../include/memory/ResourceManager.h"
#include "../../include/io/Keyboard.h"

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

SpriteRenderer* Renderer;
GameObject* Player;

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
	
	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

	ResourceManager::createTexture("background.jpg", false);
	ResourceManager::createTexture("awesomeface.png", true);
	ResourceManager::createTexture("block.png", false);
	ResourceManager::createTexture("block_solid.png", false);
	ResourceManager::createTexture("paddle.png", true);

	// load levels
	GameLevel one;		one.load(ResourceManager::getLevelPath("one.lvl").c_str(), width, height / 2);
	GameLevel two;		two.load(ResourceManager::getLevelPath("two.lvl").c_str(), width, height / 2);
	GameLevel three;	three.load(ResourceManager::getLevelPath("three.lvl").c_str(), width, height / 2);
	GameLevel four;		four.load(ResourceManager::getLevelPath("four.lvl").c_str(), width, height / 2);
	Levels.push_back(one);
	Levels.push_back(two);
	Levels.push_back(three);
	Levels.push_back(four);

	level = 0;

	// Player details
	glm::vec2 playerPos = glm::vec2(width / 2.0f - PLAYER_SIZE.x / 2.0f, height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexure("paddle.png"));
}

void Game::processInput(float dt){
	if(State == GAME_ACTIVE){
		float velocity = PLAYER_VELOCITY * dt;
		if (Keyboard::key(GLFW_KEY_A) || Keyboard::key(GLFW_KEY_LEFT)) {
			if (Player->Position.x >= 0.0f)
				Player->Position.x -= velocity;
		}
		if (Keyboard::key(GLFW_KEY_D) || Keyboard::key(GLFW_KEY_RIGHT)) {
			if (Player->Position.x <= width - Player->Size.x)
				Player->Position.x += velocity;
		}
	}
}

void Game::update(float dt)
{
}

void Game::render(){
	if (State == GAME_ACTIVE) {
		Renderer->drawSprite(ResourceManager::getTexure("background.jpg"),
			glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);

		// draw Level
		Levels[level].draw(*Renderer);

		// Render player
		Player->draw(*Renderer);
	}
}
