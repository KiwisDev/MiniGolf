#include "sprite_renderer.h"

float vertices[] = {
    // pos      // tex
    0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
    0.0f, 1.0f, 0.0f, 1.0f, // top-left
    1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
    1.0f, 1.0f, 1.0f, 1.0f, // top-right
};

SpriteRenderer::SpriteRenderer(Shader* shader) {
    this->shader = shader;
    this->quadVAO = 0;
    this->quadVBO = 0;

    glGenBuffers(1, &this->quadVBO);

    glGenVertexArrays(1, &this->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::drawSpite(Texture2D* texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
    this->shader->use();

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position, 0.0));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    this->shader->setMat4("model", model);
    this->shader->setVec3("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture->use();

    // Bottom left
    vertices[2] = 0.0f;
    vertices[3] = 0.0f;

    // Top left
    vertices[6] = 0.0f;
    vertices[7] = 1.0f;

    // Bottom right
    vertices[10] = 1.0f;
    vertices[11] = 0.0f;

    // Top right
    vertices[14] = 1.0f;
    vertices[15] = 1.0f;

    glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void SpriteRenderer::drawSpite(TextureSheet2D* texture, int id, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
    this->shader->use();

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position, 0.0));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    this->shader->setMat4("model", model);
    this->shader->setVec3("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture->use();
    texture->updateVertices(vertices, id);

    glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}