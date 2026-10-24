#include <tuple>

#include "../../include/game/game.h"
#include "../../include/graphics/models/spriteRenderer.h"
#include "../../include/memory/ResourceManager.h"
#include "../../include/io/Keyboard.h"
#include "../../include/game/ballObject.h"

// Player
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);
GameObject* Player;

// Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;
BallObject* Ball;

SpriteRenderer* Renderer;

typedef std::tuple<bool, Direction, glm::vec2> Collision;

Direction vectorDirection(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f), // down
		glm::vec2(-1.0f, 0.0f)	// left
	};

	float max = 0.0f;
	unsigned int best_match = 0;
	for (unsigned int i = 0; i < 4; i++) {
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max) {
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

Collision checkCollision(BallObject& one, GameObject& two) {
	// get center point circle first
	glm::vec2 center(one.Position + one.radius);
	// calculate half AABB infor(center, half-extensts)
	glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
	glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	glm::vec2 closest = clamped + aabb_center;

	difference = center - closest;
	if (glm::length(difference) <= one.radius) {
		return std::make_tuple(true, vectorDirection(difference), difference);
	}
	else
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_ACTIVE), Keys(), width(width), height(height){

}

Game::~Game(){
	delete Renderer;
	delete Player;
	delete Ball;
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

	// ball details
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexure("awesomeface.png"));
}

void Game::processInput(float dt){
	if(State == GAME_ACTIVE){
		float velocity = PLAYER_VELOCITY * dt;
		if (Keyboard::key(GLFW_KEY_A) || Keyboard::key(GLFW_KEY_LEFT)) {
			if (Player->Position.x >= 0.0f) {
				Player->Position.x -= velocity;
				if (Ball->stuck)
					Ball->Position.x -= velocity;
			}
		}
		if (Keyboard::key(GLFW_KEY_D) || Keyboard::key(GLFW_KEY_RIGHT)) {
			if (Player->Position.x <= width - Player->Size.x) {
				Player->Position.x += velocity;
				if (Ball->stuck)
					Ball->Position.x += velocity;
			}
		}

		if (Keyboard::keyWentDown(GLFW_KEY_SPACE))
			Ball->stuck = false;
	}
}

void Game::update(float dt){
	Ball->move(dt, width);
	doCollisions();

	if (Ball->Position.y >= height) {
		resetLevel();
		resetPlayer();
	}
}

void Game::render(){
	if (State == GAME_ACTIVE) {
		Renderer->drawSprite(ResourceManager::getTexure("background.jpg"),
			glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);

		// draw Level
		Levels[level].draw(*Renderer);

		// Render player
		Player->draw(*Renderer);

		// Render player
		Ball->draw(*Renderer);
	}
}

void Game::doCollisions(){
	for (GameObject& box : Levels[level].Bricks) {
		if (!box.Destroyed) {
			Collision collision = checkCollision(*Ball, box);
			if (std::get<0>(collision)) {
				if (!box.IsSolid)
					box.Destroyed = true;

				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT) { // horizontal collision
					Ball->Velocity.x = -Ball->Velocity.x;
					// relocate
					float penetration = Ball->radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						Ball->Position.x += penetration;
					else
						Ball->Position.x -= penetration;
				}
				else {
					Ball->Velocity.y = -Ball->Velocity.y;

					float penetration = Ball->radius - std::abs(diff_vector.y);
					if (dir == UP)
						Ball->Position.y -= penetration;
					else
						Ball->Position.y += penetration;
				}
			}
		}
	}

	Collision result = checkCollision(*Ball, *Player);
	if(!Ball->stuck && std::get<0>(result)){
		float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
		float distance = (Ball->Position.x + Ball->radius) - centerBoard;
		float percentage = distance / (Player->Size.x / 2.0f);

		float strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
	}
}

void Game::resetLevel(){
	if (level == 0)
		Levels[0].load(ResourceManager::getLevelPath("one.lvl").c_str(), width, height / 2);
	else if (level == 1)
		Levels[1].load(ResourceManager::getLevelPath("two.lvl").c_str(), width, height / 2);
	else if (level == 2)
		Levels[2].load(ResourceManager::getLevelPath("three.lvl").c_str(), width, height / 2);
	else if (level == 3)
		Levels[3].load(ResourceManager::getLevelPath("four.lvl").c_str(), width, height / 2);

}

void Game::resetPlayer(){
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(width / 2.0f - PLAYER_SIZE.x / 2.0f, height - PLAYER_SIZE.y);
	Ball->reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

