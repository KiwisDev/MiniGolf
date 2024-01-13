#include "texture_sheet2D.h"

TextureSheet2D::TextureSheet2D(Texture2D* texture, int rows, int columns) {
	this->texture = texture;
	this->rows = rows;
	this->columns = columns;
}

void TextureSheet2D::updateVertices(float vertices[16], int id) {
	// Check if id is not out of bound
	if (rows * columns >= id + 1)
		return;

	float row_pad = 1.0 / (float) rows;
	float col_pad = 1.0 / (float)columns;

	int origin_row = (id / columns) * row_pad;
	int origin_col = (id % columns) * col_pad;

	// Bottom left
	vertices[2] = origin_row + row_pad;
	vertices[3] = origin_col;

	// Top left
	vertices[6] = origin_row;
	vertices[7] = origin_col;

	// Bottom right
	vertices[10] = origin_row + row_pad;
	vertices[11] = origin_col + col_pad;

	// Top right
	vertices[14] = origin_row;
	vertices[15] = origin_col + col_pad;
}
