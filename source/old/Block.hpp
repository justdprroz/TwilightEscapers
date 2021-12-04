#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <TextureManager.hpp>

class Block {
public:
    Block(){
    };
    virtual void draw(sf::RenderWindow& p_win, const int p_x, const int p_y, const int p_tileSize){
        sf::Sprite cell;
        sf::Vector2u tSize = m_texture->getSize();
        cell.setScale(sf::Vector2f((float)p_tileSize / (float)tSize.x, (float)p_tileSize / (float)tSize.y));
        cell.setTexture(*m_texture);
        cell.setPosition(p_x * p_tileSize, p_y * p_tileSize);
        p_win.draw(cell);
    };
protected:
    sf::Texture* m_texture;
};

class DirtBlock : public Block {
public:
    DirtBlock(){
        m_texture = tm.getTexture("dirt");
    };
protected:
};

class StoneBlock : public Block {
public:
    StoneBlock(){
        m_texture = tm.getTexture("stone");
    };
protected:
};