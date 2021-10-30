#pragma once

#include <utility>

class Entity {
public:
    // constructor
    Entity(){
        
    };
    // setting textures
    void setTextureFront(const sf::Texture& p_texture){
        m_textureFront = p_texture;
    }
    void setTextureLeft(const sf::Texture& p_texture){
        m_textureLeft = p_texture;
    }
    void setTextureBack(const sf::Texture& p_texture){
        m_textureBack = p_texture;
    }
    void setTextureRight(const sf::Texture& p_texture){
        m_textureRight = p_texture;
    }

    void move(float p_timeStep){
        m_position.first += m_velocity.first * p_timeStep;
        m_position.second += m_velocity.second * p_timeStep;
    }
    
    void draw(sf::RenderWindow &p_win, const int p_tileSize) {
        float locscaleX = (float)p_tileSize / (float)m_textureCurrent->getSize().x;
        float locscaleY = (float)p_tileSize / (float)m_textureCurrent->getSize().y;
        sf::Sprite cell;
        cell.setTexture(*m_textureCurrent);
        cell.setPosition(m_position.first * p_tileSize, m_position.second * p_tileSize);
        cell.setScale(sf::Vector2f(locscaleX, locscaleY));
        p_win.draw(cell);
    }

protected:
    sf::Texture m_textureFront;
    sf::Texture m_textureLeft;
    sf::Texture m_textureBack;
    sf::Texture m_textureRight;
    sf::Texture* m_textureCurrent = &m_textureFront;
    std::pair<float, float> m_position;
    std::pair<float, float> m_velocity = {0, 0};
    float m_speed = 10;
};