#include "font_renderer.h"

FontRenderer::FontRenderer(Shader* s, const char* font_path, int width, int height) {
	this->shader = s;
	this->width = width;
	this->height = height;

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << "\n -- --------------------------------------------------- -- " << std::endl;
		exit(-1);
	}

	FT_Face face;
	if (FT_New_Face(ft, font_path, 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << "\n -- --------------------------------------------------- -- " << std::endl;
		exit(-1);
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int i = 32; i < 256; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << "\n -- --------------------------------------------------- -- " << std::endl;
			continue;
		}

		unsigned int tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			tex,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		this->chars.insert(std::pair<char, Character>(i, character));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void FontRenderer::renderText(std::string text, float x, float y, float scale, glm::vec3 color) {
	this->shader->use();
	this->shader->setMat4("projection", glm::ortho(0.0f, (float)this->width, 0.0f, (float)this->height));

	this->shader->setVec3("textColor", color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	std::string::const_iterator it;
	for (it = text.begin(); it != text.end(); it++) {
		Character ch = this->chars[*it];

		float xPos = x + ch.bearing.x * scale;
		float yPos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		float verticies[6][4] = {
			{ xPos,     yPos + h,   0.0f, 0.0f },
			{ xPos,     yPos,       0.0f, 1.0f },
			{ xPos + w, yPos,       1.0f, 1.0f },

			{ xPos,     yPos + h,   0.0f, 0.0f },
			{ xPos + w, yPos,       1.0f, 1.0f },
			{ xPos + w, yPos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.texID);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticies), verticies);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}