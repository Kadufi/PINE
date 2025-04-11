#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <vector> 
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
/*
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
    {"1D", GL_TEXTURE_1D},          //X
    {"2D", GL_TEXTURE_2D},          //ok
    {"3D", GL_TEXTURE_3D},          //x
    {"TC_MAP", GL_TEXTURE_CUBE_MAP} //ok
};
*/

class Texture
{
public:
    unsigned int texture;
    GLenum textureType;
    

// Construtor mais pŕatico ================================================================================================================
    Texture(GLenum type, GLenum filter, GLenum wrap)
    {
        //isValidTextureParam(type);
        //isValidTextureParam(filter);
        //isValidTextureParam(wrap);
        textureType = type;
        glGenTextures(1, &texture);
        glBindTexture(textureType, texture);

        setWrap(wrap);
        setFiltering(filter);

    }

    void setWrap(GLenum wrap)
    {
        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrap);	
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrap);
        if (wrap == GL_CLAMP_TO_BORDER) {
            glTexParameterfv(textureType, GL_TEXTURE_BORDER_COLOR, TextureBorderColor);
        }
    }

    void setFiltering(GLenum filter)
    {
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filter);
    }

    void activateAndBind(int textureUnit){
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(textureType, texture);
    }

//Construtor mais detalhado=====================================================================================================================================
Texture(GLenum type)
    {
        //isValidTextureParam(type);
        textureType = type;
        glGenTextures(1, &texture);
        glBindTexture(textureType, texture);
    }

    void setWrap(GLenum wrapS, GLenum wrapT)
    {
        //isValidTextureParam(wrapS);
        //isValidTextureParam(wrapT);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapS);	
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapT);
        if (wrapS == GL_CLAMP_TO_BORDER || wrapT == GL_CLAMP_TO_BORDER) {
            glTexParameterfv(textureType, GL_TEXTURE_BORDER_COLOR, TextureBorderColor);
        }
    }

    void setFiltering(GLenum filterMIN, GLenum filterMAG)
    {
        //isValidTextureParam(filterMAG);
        //isValidTextureParam(filterMIN);
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, filterMIN);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filterMAG);
    }

    void setCubeWrap(GLenum  wrapS, GLenum wrapT, GLenum wrapR)
    {
        //isValidTextureParam(wrapS);
        //isValidTextureParam(wrapT);
        //isValidTextureParam(wrapR);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);

        if (wrapS == GL_CLAMP_TO_BORDER || wrapT == GL_CLAMP_TO_BORDER || wrapR == GL_CLAMP_TO_BORDER) {
            glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, TextureBorderColor);
        }
    }

    void setCubeFiltering(GLenum minFilter, GLenum magFilter)//mesma coisa que o setFiltering
    {
        //isValidTextureParam(minFilter);
        //isValidTextureParam(magFilter);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
    }

//==========================================================================================================================================
    void setImg(const char* texturePath)
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

    void setCubeDefaults()
    {
        setCubeWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        setCubeFiltering(GL_LINEAR, GL_LINEAR);
    }

    void setCubeImg(const std::vector<std::string>& faces)
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
            }
            else
            {
                std::cout << "Erro ao carregar imagem da face " << i << ": " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
    }


    /*
    void isValidTextureParam(const std::string& str) {
        bool isValid = TextureWrapModes.count(str) > 0 ||
                       TextureFilterModes.count(str) > 0 ||
                       TextureTypes.count(str) > 0;
    
        if (!isValid) {
            std::cerr << "Erro de digitação, campo textura:  " << str << std::endl;
        }
    }
    */

    ~Texture(){};

};

#endif // TEXTURE_HPP
