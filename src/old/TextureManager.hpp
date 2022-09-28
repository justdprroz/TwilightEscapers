#pragma once

#include <map>
#include <string>
#include <SFML/Graphics/Texture.hpp>

class TextureManager {
public:
    TextureManager(){
        m_textures["dirt"] = sf::Texture();
        m_textures["dirt"].loadFromFile("textures/main/Tile/dirt.png");
        m_textures["stone"] = sf::Texture();
        m_textures["stone"].loadFromFile("textures/main/Tile/stone.png");
    }
    sf::Texture* getTexture(std::string p_name){
        return &m_textures[p_name];
    }
private:
    std::map<std::string, sf::Texture> m_textures;
};

TextureManager tm;