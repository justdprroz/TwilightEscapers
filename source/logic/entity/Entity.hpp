#pragma once

#include <logic/base/Base.hpp>
#include <utility>

class Entity : public Base {
public:
    // constructor
    Entity() {
        m_className = "Entity";
    }

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
    
protected:
    sf::Texture m_textureFront;
    sf::Texture m_textureLeft;
    sf::Texture m_textureBack;
    sf::Texture m_textureRight;
    sf::Texture m_textureCurrent;
    std::pair<float, float> m_position;
    std::pair<float, float> m_velocity = {0, 0};
    float m_speed = 10;
};