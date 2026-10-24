#pragma once

#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "gameLevel.h"

// Represents current state of game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


class Game {
public: 
	GameState	State;
	bool		Keys[1024];
	unsigned int width, height;

	std::vector<GameLevel> Levels;
	unsigned int level;

	Game(unsigned int width, unsigned int height);
	~Game();

	void init();
	// game loop
	void processInput(float dt);
	void update(float dt);
	void render();
};

#endif // !GAME_H

