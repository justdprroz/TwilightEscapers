// Copyright 2021-2022 JustDprroz

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
        LoadBlockTextures();
        LoadEntityTextures();
        LoadTilesetsBlocks();
    }
    void SimpleRenderChunk(Chunk& p_chunk) {
        if (!p_chunk.IsGenerated()) return;
        sf::Vector2i origin = p_chunk.getOrigin();
        for(int i = 0; i < 16; i++){
            for(int j = 0; j < 16; j++){
                sf::Sprite cell;
                int id = p_chunk.GetBlock({i, j}).GetId();
                cell.setTexture(m_texturesBlocks[id]);
                cell.setPosition(origin.x * 256 + i * 16, origin.y * 256 + j * 16);
                m_windowPtr->draw(cell);
            }
        }
    }
    void ChunkRenderBiome(Chunk& p_chunk) {
        if (!p_chunk.IsGenerated()) return;
        sf::Vector2i origin = p_chunk.getOrigin();
        for(int i = 0; i < 16; i++){
            for(int j = 0; j < 16; j++){
                int g_x = origin.x * 16 + i;
                int g_y = origin.y * 16 + j;
                int id = p_chunk.GetBlock({i, j}).GetId();
                int biome = p_chunk.GetBlock({i, j}).GetBiome();
                sf::Sprite cell;
                cell.setTexture(m_tilesetsBlocks[biome]);
                cell.setPosition(g_x * 16, g_y * 16);
                if (id == 0) {
                    cell.setTextureRect(sf::IntRect(64, 0 + mod((g_y + g_x * 2), 4) * 16, 16, 16));
                }
                if (id == 1) {
                    cell.setTextureRect(sf::IntRect(0, 16 + mod((g_y + g_x * 2), 4) * 16, 16, 16));
                }
                m_windowPtr->draw(cell);
            }
        }
    }
    void SimpleRenderEntities(std::vector<Entity>& p_entities) {
        for(Entity it : p_entities) {
            sf::Sprite cell;
            int id = it.GetId();
            cell.setTexture(m_texturesEntities[id]);
            cell.setTextureRect(sf::IntRect(0, 0, 16, 16));
            sf::Vector2f pos = it.GetPosition();
            cell.setPosition(pos.x * 16, pos.y * 16);
            m_windowPtr->draw(cell);
        }
    }
private:
    void LoadBlockTextures() {
        // m_texturesBlocks[0].loadFromFile("assets/textures/tile/stone.png");
        // m_texturesBlocks[1].loadFromFile("assets/textures/tile/dirt.png");
    }
    void LoadEntityTextures() {
        m_texturesEntities[0].loadFromFile("assets/textures/entity/office.png");
        m_texturesEntities[1].loadFromFile("assets/textures/entity/guard.png");
    }
    void LoadTilesetsBlocks() {
        m_tilesetsBlocks[0].loadFromFile("assets/textures/tile/tileset_0.png");
        m_tilesetsBlocks[1].loadFromFile("assets/textures/tile/tileset_1.png");
        m_tilesetsBlocks[2].loadFromFile("assets/textures/tile/tileset_2.png");
    }
    sf::Texture m_texturesBlocks[2];
    sf::Texture m_tilesetsBlocks[3];
    sf::Texture m_texturesEntities[2];
    sf::RenderWindow* m_windowPtr;
    World* m_currentWorld;
};

#endif