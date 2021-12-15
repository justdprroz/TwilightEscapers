// Copyright 2021 JustDprroz
#ifndef SOURCE_RENDER_HPP_
#define SOURCE_RENDER_HPP_

#include <SFML/Graphics.hpp>
#include <World.hpp>

class Renderer {
public:
    void AttachWindow(sf::RenderWindow *p_win){
        m_windowPtr = p_win;
    }
    void AttachWorld(World *p_world) {
        m_currentWorld = p_world;
    }
    void LoadTextures() {
        m_textures[0].loadFromFile("textures/stone.png");
        m_textures[1].loadFromFile("textures/dirt.png");
    }
    void SimpleRenderChunk(Chunk& p_chunk) {
        sf::Vector2i origin = p_chunk.getOrigin();
        for(int i = 0; i < 16; i++){
            for(int j = 0; j < 16; j++){
                sf::Sprite cell;
                int id = p_chunk.GetBlock({i, j})->m_id;
                cell.setTexture(m_textures[id]);
                cell.setPosition(origin.x * 256 + i * 16, origin.y * 256 + j * 16);
                m_windowPtr->draw(cell);
            }
        }
    }
private:
    sf::Texture m_textures[2];
    sf::RenderWindow* m_windowPtr;
    World* m_currentWorld;
};

#endif