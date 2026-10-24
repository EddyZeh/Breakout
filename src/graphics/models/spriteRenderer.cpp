#include "../../../include/graphics/models/spriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader)
: shader(shader) {
    initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color){
    shader.activate();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0));

    shader.setMat4("model", model);
    shader.set3Float("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    VAO.bind();
    VAO.draw(GL_TRIANGLES, 6);
    ArrayObject::clear();
    
}

void SpriteRenderer::initRenderData(){
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    VAO.generate();
    VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);

    VAO.bind();
    VAO["VBO"].bind();
    int size = static_cast<int>(sizeof(vertices) / sizeof(vertices[0]));
    VAO["VBO"].setData<float>(size, vertices, GL_STATIC_DRAW);
    VAO["VBO"].setAttPointer<float>(0, 4, GL_FLOAT, 4, 0);
    ArrayObject::clear();
}
