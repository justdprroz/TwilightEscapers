// Copyright 2021-2022 JustDprroz

#ifndef SOURCE_RENDER_HPP_
#define SOURCE_RENDER_HPP_

#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <Utils.hpp>

class Renderer {
public:
    void AttachWindow(sf::RenderWindow *p_win);
    void AttachWorld(World *p_world);
    void LoadTextures();
    void SimpleRenderChunk(Chunk& p_chunk);
    void ChunkRenderBiome(Chunk& p_chunk);
    void SimpleRenderEntities(std::vector<Entity>& p_entities);
    sf::Texture* GetTileset();
private:
    void LoadBlockTextures();
    void LoadEntityTextures();
    void LoadTilesetsBlocks();
    sf::Texture m_tilesetsBlocks[3];
    sf::Texture m_blockTileset;
    sf::Texture m_texturesEntities[2];
    sf::RenderWindow* m_windowPtr;
    World* m_currentWorld;
};

class RenderChunk : public sf::Drawable, public sf::Transformable {
public:
    void Update(Chunk& p_chunk, Renderer& p_renderer);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    sf::VertexArray m_vertices;
    sf::Texture* m_tileset;
};

class RenderWorld : public sf::Drawable, public sf::Transformable {
public:
    void Update(World& p_world, Renderer& p_renderer);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    std::map<std::pair<int, int>, RenderChunk> m_chunks;
};

#endif