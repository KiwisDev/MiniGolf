#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <glad/glad.h>

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader.h"
#include "texture2D.h"
#include "font_renderer.h"

class RessourceManager {
private:
	RessourceManager();

public:
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;
	static std::map<std::string, FontRenderer> fonts;

	static Shader* loadShader(std::string name, const char* vertexPath, const char* fragmentPath);
	static Shader* getShader(std::string name) {
		return &shaders.at(name);
	}

	static Texture2D* loadTexture(std::string name, const char* path, bool flip = false, bool isRGBA = false, GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT, GLint minFilter = GL_NEAREST_MIPMAP_NEAREST, GLint magFilter = GL_NEAREST);
	static Texture2D* getTexture(std::string name) {
		return &textures.at(name);
	}

	static FontRenderer* loadFont(std::string name, Shader* s, const char* font_path, int width, int height);
	static void resizeFonts(int width, int height) {
		for (std::map<std::string, FontRenderer>::iterator it = fonts.begin(); it != fonts.end(); ++it)
			it->second.resize(width, height);
	}
	static FontRenderer* getFont(std::string name) {
		return &fonts.at(name);
	}
};

#endif