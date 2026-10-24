#include "../../include/game/ballObject.h"

BallObject::BallObject()
			: GameObject(), radius(12.5f), stuck(true){}

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture sprite)
			: GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), 
			  radius(radius), stuck(true) { }

glm::vec2 BallObject::move(float dt, unsigned int window_width){
	if (!stuck) {
		// move the ball
		Position += Velocity * dt;
		// check if outside window bounds
		if (Position.x <= 0.0f) {
			Velocity.x = -Velocity.x;
			Position.x = 0.0f;
		}
		else if (Position.x + Size.x >= window_width) {
			Velocity.x = -Velocity.x;
			Position.x = window_width - Size.x;
		}
		if (Position.y <= 0.0f) {
			Velocity.y = -Velocity.y;
			Position.y = 0.0f;
		}
	}
	return Position;
}

void BallObject::reset(glm::vec2 position, glm::vec2 velocity){
	Position = position;
	Velocity = velocity;
	stuck = true;
}
