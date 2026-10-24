#include "../../include/game/gameLevel.h"
#include "../../include/memory/ResourceManager.h"

#include <string>
#include <fstream>
#include <sstream>

void GameLevel::load(const char* file, unsigned int levelWidth, unsigned int levelHeight) {
	Bricks.clear();

	// load from file
	unsigned int tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;
	if (fstream) {
		while (std::getline(fstream, line)) {
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode)
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::draw(SpriteRenderer& renderer) {
	for (GameObject& tile : Bricks) {
		if (!tile.Destroyed)
			tile.draw(renderer);
	}
}

bool GameLevel::isCompleted() {
	for (auto& tile : Bricks) {
		if (!tile.IsSolid && !tile.Destroyed)
			return false;
	}
	return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight){
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();
	float unit_width = levelWidth / static_cast<float>(width);
	float unit_height = levelHeight / height;

	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			if (tileData[y][x] == 1) { // solid
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size,
					ResourceManager::getTexure("block_solid.png"),
					glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = true;
				Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1) {
				glm::vec3 color = glm::vec3(1.0f);
				switch (tileData[y][x]) {
				case 2:
					color = glm::vec3(0.2f, 0.6f, 1.0f);
					break;
				case 3:
					color = glm::vec3(0.0f, 0.7f, 0.0f);
					break;
				case 4:
					color = glm::vec3(0.8f, 0.8f, 0.4f);
					break;
				case 5:
					color = glm::vec3(1.0f, 0.5f, 0.0f);
					break;
				}

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				Bricks.push_back(GameObject(pos, size, ResourceManager::getTexure("block.png"), color));
			}
		}
	}
}
