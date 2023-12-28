#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>

#include "shader.h"

typedef struct {
	unsigned int texID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
} Character;

class FontRenderer {
private:
	Shader* shader;
	unsigned int VAO, VBO;
	std::map<char, Character> chars;
	int width, height;

public:
	FontRenderer(Shader* s, const char* font_path, int width, int height);

	void renderText(std::string text, float x, float y, float scale, glm::vec3 color);

	void resize(int w, int h) {
		width = w;
		height = h;
	}
};

#endif