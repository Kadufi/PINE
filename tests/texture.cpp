#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float TextureBorderColor[4] = {0.0f, 1.0f, 0.0f, 1.0f};

void Texture::setBorderColor(float R, float G, float B, float A) {
    TextureBorderColor[0] = R;
    TextureBorderColor[1] = G;
    TextureBorderColor[2] = B;
    TextureBorderColor[3] = A;
}

//=========================================================================================================
//Mais prática
Texture::Texture(GLenum type, GLenum filter, GLenum wrap)
{
    textureType = type;
    glGenTextures(1, &texture);
    glBindTexture(textureType, texture);

    setWrap(wrap);
    setFiltering(filter);
}

void Texture::setWrap(GLenum wrap)
{
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrap);    
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrap);
    if (wrap == GL_CLAMP_TO_BORDER) {
        glTexParameterfv(textureType, GL_TEXTURE_BORDER_COLOR, TextureBorderColor);
    }
}

void Texture::setFiltering(GLenum filter)
{
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filter);
}

//===================================================================================================
//Mais Detelhada
Texture::Texture(GLenum type)
{
    textureType = type;
    glGenTextures(1, &texture);
    glBindTexture(textureType, texture);
}

void Texture::setWrap(GLenum wrapS, GLenum wrapT)
{
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapS);    
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapT);
    if (wrapS == GL_CLAMP_TO_BORDER || wrapT == GL_CLAMP_TO_BORDER) {
        glTexParameterfv(textureType, GL_TEXTURE_BORDER_COLOR, TextureBorderColor);
    }
}

void Texture::setFiltering(GLenum filterMIN, GLenum filterMAG)
{
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, filterMIN);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filterMAG);
}

void Texture::setCubeWrap(GLenum wrapS, GLenum wrapT, GLenum wrapR)
{
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);

    if (wrapS == GL_CLAMP_TO_BORDER || wrapT == GL_CLAMP_TO_BORDER || wrapR == GL_CLAMP_TO_BORDER) {
        glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, TextureBorderColor);
    }
}

//equivalente a set setFiltering
void Texture::setCubeFiltering(GLenum minFilter, GLenum magFilter)
{
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
}

//==========================================================================

void Texture::activateAndBind(int textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(textureType, texture);
}

void Texture::setImg(const char* texturePath)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 4) format = GL_RGBA;
        else if (nrChannels == 1) format = GL_RED;

        if (textureType == GL_TEXTURE_2D) {
            glTexImage2D(textureType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(textureType);
        }
    }
    else
    {
        std::cout << "Failed to load texture: " << texturePath << std::endl;
    }
    stbi_image_free(data);
}

//https://www.ogldev.org/www/tutorial25/tutorial25.html

void Texture::setCubeDefaults()
{
    setCubeWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    setCubeFiltering(GL_LINEAR, GL_LINEAR);
}

void Texture::setCubeImg(const std::vector<std::string>& faces)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = GL_RGB;
            if (nrChannels == 4) format = GL_RGBA;
            else if (nrChannels == 1) format = GL_RED;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            //setCubeDefaults();
        }
        else
        {
            std::cout << "Erro ao carregar imagem da face " << i << ": " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
}

Texture::~Texture() {}
