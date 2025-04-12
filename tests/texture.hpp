#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

class Texture
{
public:
    unsigned int texture;
    GLenum textureType;

    // Construtor básico
    Texture(GLenum type, GLenum filter, GLenum wrap);
    // Construtor mais detalhado
    Texture(GLenum type);
    
    void setWrap(GLenum wrap);
    void setWrap(GLenum wrapS, GLenum wrapT);
    void setFiltering(GLenum filter);
    void setFiltering(GLenum filterMIN, GLenum filterMAG);
    
    void activateAndBind(int textureUnit);
    
    void setCubeWrap(GLenum wrapS, GLenum wrapT, GLenum wrapR);
    void setCubeFiltering(GLenum minFilter, GLenum magFilter);
    
    void setImg(const char* texturePath);
    void setCubeDefaults();
    void setCubeImg(const std::vector<std::string>& faces);
    
    ~Texture();
    void setBorderColor(float R, float G, float B, float A);
};

#endif // TEXTURE_HPP
