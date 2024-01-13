#ifndef TEXTURE_SHEET_2D_H
#define TEXTURE_SHEET_2D_H

#include <glad/glad.h>
#include "texture2D.h"

class TextureSheet2D {
private:
	Texture2D* texture;
	int rows, columns;

public:
	TextureSheet2D(Texture2D* texture, int rows, int columns);
	void updateVertices(float vertices[16], int id); // ID From top left starting at 0
	void use() {
		texture->use();
	}
};

#endif