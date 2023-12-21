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

class RessourceManager {
private:
	RessourceManager();

public:
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;

	static Shader* loadShader(std::string name, const char* vertexPath, const char* fragmentPath);
	static Shader* getShader(std::string name) {
		return &shaders.at(name);
	}

	static Texture2D* loadTexture(std::string name, const char* path, bool flip = false, bool isRGBA = false, GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT, GLint minFilter = GL_NEAREST_MIPMAP_NEAREST, GLint magFilter = GL_NEAREST);
	static Texture2D* getTexture(std::string name) {
		return &textures.at(name);
	}

	static void clear();
};

#endif