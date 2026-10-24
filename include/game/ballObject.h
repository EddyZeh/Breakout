#pragma once

#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include "gameObject.h"

class BallObject : public GameObject {
public:
	// ball state
	float	radius;
	bool	stuck;

	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture sprite);

	glm::vec2 move(float dt, unsigned int window_width);
	void reset(glm::vec2 position, glm::vec2 velocity);
};

#endif // !BALLOBJECT_H
