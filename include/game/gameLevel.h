#pragma once

#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <vector>

#include "gameObject.h"

class GameLevel {
public:
	// level state
	std::vector<GameObject> Bricks;

	GameLevel(){}
	// loads from file
	void load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	// render level
	void draw(SpriteRenderer& renderer);
	// check if level is completed (all non solid tiles are destroyed)
	bool isCompleted();

private:
	// initialize level from tile data
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif // !GAMELEVEL_H

