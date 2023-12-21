#include "texture2D.h"

Texture2D::Texture2D(int width, int height, int nbChannels, unsigned int texture) {
	this->width = width;
	this->height = height;
	this->nbChannels = nbChannels;
	this->texture = texture;
}