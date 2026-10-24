#pragma once

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "../Shader.h"
#include "../Texture.h"
#include "../../memory/glMemory.hpp"

class SpriteRenderer {
public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	void drawSprite(Texture& texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));

private:
	Shader shader;
	ArrayObject VAO;
	unsigned int quadVAO;

	void initRenderData();
};

#endif // !SPRITERENDERER_H

