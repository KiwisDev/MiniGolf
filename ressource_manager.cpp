#include "ressource_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::map<std::string, Shader> RessourceManager::shaders;
std::map<std::string, Texture2D> RessourceManager::textures;

Shader* RessourceManager::loadShader(std::string name, const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    shaders.insert(std::pair<std::string, Shader>(name, Shader(vShaderCode, fShaderCode)));

    return &shaders.at(name);
}

Texture2D* RessourceManager::loadTexture(std::string name, const char* path, bool flip, bool isRGBA, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter) {
    int width = 0;
    int height = 0;
    int nbChannels = 0;
    unsigned int texture;

    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(path, &width, &height, &nbChannels, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, (isRGBA) ? GL_RGBA : GL_RGB, width, height, 0, (isRGBA) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    textures.insert(std::pair<std::string, Texture2D>(name, Texture2D(width, height, nbChannels, texture)));

    return &textures.at(name);
}