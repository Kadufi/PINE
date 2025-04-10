#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

float TextureBorderColor[4] = {0.0f, 1.0f, 0.0f, 1.0f};

void setBorderColor(float R, float G, float B, float A){
    TextureBorderColor[0] = R;
    TextureBorderColor[1] = G;
    TextureBorderColor[2] = B;
    TextureBorderColor[3] = A;
};

//=============================================================
std::unordered_map<std::string, GLenum> TextureWrapModes = {
    {"REPEAT", GL_REPEAT},
    {"MIRRORED", GL_MIRRORED_REPEAT},
    {"EDGE", GL_CLAMP_TO_EDGE},
    {"BORDER", GL_CLAMP_TO_BORDER}
};

//=============================================================
std::unordered_map<std::string, GLenum> TextureFilterModes = {
    {"NEAREST", GL_NEAREST},
    {"LINEAR", GL_LINEAR},
    {"NMN", GL_NEAREST_MIPMAP_NEAREST},
    {"LMN", GL_LINEAR_MIPMAP_NEAREST},
    {"NML", GL_NEAREST_MIPMAP_LINEAR},
    {"LML", GL_LINEAR_MIPMAP_LINEAR}
};

//=============================================================
std::unordered_map<std::string, GLenum> TextureTypes = {
    {"1D", GL_TEXTURE_1D},
    {"2D", GL_TEXTURE_2D},
    {"3D", GL_TEXTURE_3D},
    {"TC_MAP", GL_TEXTURE_CUBE_MAP}
};


class Texture
{
public:
    unsigned int texture;
    GLenum textureType;
    

// Construtor mais pŕatico ================================================================================================================
    Texture(std::string type, std::string filter, std::string wrap)
    {
        isValidTextureParam(type);
        isValidTextureParam(filter);
        isValidTextureParam(wrap);
        textureType = TextureTypes[type];
        glGenTextures(1, &texture);
        glBindTexture(textureType, texture);

        setWrap(wrap);
        setFiltering(filter);

    }

    void setWrap(std::string wrap)
    {
        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, TextureWrapModes[wrap]);	
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, TextureWrapModes[wrap]);
        if(wrap == "BORDER"){
            glTexParameterfv(textureType, GL_TEXTURE_BORDER_COLOR, TextureBorderColor);
        }
    }

    void setFiltering(std::string filter)
    {
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, TextureFilterModes[filter]);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, TextureFilterModes[filter]);
    }

    void activateAndBind(int textureUnit){
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(textureType, texture);
    }

//Construtor mais detalhado=====================================================================================================================================
Texture(std::string type)
    {
        isValidTextureParam(type);
        textureType = TextureTypes[type];
        glGenTextures(1, &texture);
        glBindTexture(textureType, texture);
    }

    void setWrap(std::string wrapS, std::string wrapT)
    {
        isValidTextureParam(wrapS);
        isValidTextureParam(wrapT);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, TextureWrapModes[wrapS]);	
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, TextureWrapModes[wrapT]);
        if(wrapS == "BORDER" || wrapT == "BORDER"){
            glTexParameterfv(textureType, GL_TEXTURE_BORDER_COLOR, TextureBorderColor);
        }
    }

    void setFiltering(std::string filterMIN, std::string filterMAG)
    {
        isValidTextureParam(filterMAG);
        isValidTextureParam(filterMIN);
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, TextureFilterModes[filterMIN]);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, TextureFilterModes[filterMAG]);
    }

//==========================================================================================================================================
    void setImg(const char* texturePath){

        int width, height, nrChannels;
        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

        if (data)
        {
            GLenum format = GL_RGB; 
            if (nrChannels == 4) 
                format = GL_RGBA;
            
            glTexImage2D(textureType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(textureType);
        }
        else
        {
            std::cout << "Failed to load texture: " << texturePath << std::endl;
        }
        stbi_image_free(data);

    }

    void isValidTextureParam(const std::string& str) {
        bool isValid = TextureWrapModes.count(str) > 0 ||
                       TextureFilterModes.count(str) > 0 ||
                       TextureTypes.count(str) > 0;
    
        if (!isValid) {
            std::cerr << "Erro de digitação, campo textura:  " << str << std::endl;
        }
    }

    ~Texture() {
        std::cout << "Textura " << texture << " deletada." << std::endl;
    }

};

#endif // TEXTURE_HPP
