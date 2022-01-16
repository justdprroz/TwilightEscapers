#include "Render.hpp"

void Renderer::AttachWindow(sf::RenderWindow *p_win){
    m_windowPtr = p_win;
}

void Renderer::AttachWorld(World *p_world) {
    m_currentWorld = p_world;
}

void Renderer::LoadTextures() {
    LoadBlockTextures();
    LoadEntityTextures();
    LoadTilesetsBlocks();
}

void Renderer::SimpleRenderChunk(Chunk& p_chunk) {
    if (!p_chunk.IsGenerated()) return;
    sf::Vector2i origin = p_chunk.getOrigin();
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            sf::Sprite cell;
            int id = p_chunk.m_blocks[i][j].GetId();
            cell.setTexture(m_texturesBlocks[id]);
            cell.setPosition(origin.x * 256 + i * 16, origin.y * 256 + j * 16);
            m_windowPtr->draw(cell);
        }
    }
}

void Renderer::ChunkRenderBiome(Chunk& p_chunk) {
    if (!p_chunk.IsGenerated()) return;
    sf::Vector2i origin = p_chunk.getOrigin();
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            int g_x = origin.x * 16 + i;
            int g_y = origin.y * 16 + j;
            int id = p_chunk.m_blocks[i][j].GetId();
            int biome = p_chunk.m_blocks[i][j].GetBiome();
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

void Renderer::SimpleRenderEntities(std::vector<Entity>& p_entities) {
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

void Renderer::LoadBlockTextures() {
    // m_texturesBlocks[0].loadFromFile("assets/textures/tile/stone.png");
    // m_texturesBlocks[1].loadFromFile("assets/textures/tile/dirt.png");
}
void Renderer::LoadEntityTextures() {
    m_texturesEntities[0].loadFromFile("assets/textures/entity/office.png");
    m_texturesEntities[1].loadFromFile("assets/textures/entity/guard.png");
}
void Renderer::LoadTilesetsBlocks() {
    m_tilesetsBlocks[0].loadFromFile("assets/textures/tile/tileset_0.png");
    m_tilesetsBlocks[1].loadFromFile("assets/textures/tile/tileset_1.png");
    m_tilesetsBlocks[2].loadFromFile("assets/textures/tile/tileset_2.png");
}

sf::Texture* Renderer::GetTileset(){
    return &m_tilesetsBlocks[0];
}

void RenderChunk::Update(Chunk& p_chunk, Renderer& p_renderer) {
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(16 * 16 * 4);

    m_tileset = p_renderer.GetTileset();
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            sf::Vertex* quad = &m_vertices[(i + j * 16) * 4];

            quad[0].position = sf::Vector2f(i * 16, j * 16);
            quad[1].position = sf::Vector2f((i + 1) * 16, j * 16);
            quad[2].position = sf::Vector2f((i + 1) * 16, (j + 1) * 16);
            quad[3].position = sf::Vector2f(i * 16, (j + 1) * 16);

            int id = p_chunk.m_blocks[i][j].GetId();
            int biome = p_chunk.m_blocks[i][j].GetBiome();

            int tx = 0, ty = 0;

            if (id == 0) {
                tx = 4;
                ty = 0 + mod((i + j * 2), 4);
            }
            if (id == 1) {
                tx = 0;
                ty = 1 + mod((i + j * 2), 4);
            }

            quad[0].texCoords = sf::Vector2f(tx * 16, ty * 16);
            quad[1].texCoords = sf::Vector2f((tx + 1) * 16, ty * 16);
            quad[2].texCoords = sf::Vector2f((tx + 1) * 16, (ty + 1) * 16);
            quad[3].texCoords = sf::Vector2f(tx * 16, (ty + 1) * 16);
        }
    }
    setPosition(p_chunk.getOrigin().x * 256, p_chunk.getOrigin().y * 256);
}

void RenderChunk::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = m_tileset;
    target.draw(m_vertices, states);
}

void RenderWorld::Update(World& p_world, Renderer& p_renderer){
    for(auto p_chunk : p_world.m_chunks) {
        m_chunks[p_chunk.first].Update(p_chunk.second, p_renderer);
    }
}

void RenderWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(auto p_renderChunk : m_chunks) {
        p_renderChunk.second.draw(target, states);
    }
}