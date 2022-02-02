// Copyright 2021-2022 JustDprroz

#include "Render.hpp"

TextureManager::TextureManager(std::string assets_path) {
    assets_path_ = assets_path;
    LoadBlockTileset();
}

void TextureManager::SetAssetsPath(std::string path) {
    assets_path_ = path;
}

void TextureManager::LoadBlockTileset() {
    block_tileset_.loadFromFile(assets_path_ + "/textures/tiles/tileset.png");
}

sf::Texture* TextureManager::GetBlockTilesetPtr() {
    return &block_tileset_;
}

void RenderChunk::Update(Chunk& chunk, TextureManager& texture_manager, World &world) {
    vertices_.setPrimitiveType(sf::Quads);
    vertices_.resize(kChunkSize * kChunkSize * 4);
    tileset_ptr_ = texture_manager.GetBlockTilesetPtr();
    sf::Vector2i p_origin = chunk.getOrigin();
    for (int i = 0; i < kChunkSize; i++) {
        for (int j = 0; j < kChunkSize; j++) {
            sf::Vertex* quad = &vertices_[(i + j * kChunkSize) * 4];


            int id = chunk.blocks_[i][j].GetId();
            int biome = chunk.blocks_[i][j].GetBiome();

            int g_x = p_origin.x * kChunkSize + i;
            int g_y = p_origin.y * kChunkSize + j;

            int tx = 0, ty = 0;

            if (id == 0) {
                int lx = 0;
                int ly = 0;
                if (world.GetBlock({p_origin.x * 16 + i - 1, p_origin.y * 16 + j}).GetId() == 1) {
                    lx = -1;
                }
                if (world.GetBlock({p_origin.x * 16 + i + 1, p_origin.y * 16 + j}).GetId() == 1) {
                    lx = 1;
                }
                if (world.GetBlock({p_origin.x * 16 + i, p_origin.y * 16 + j - 1}).GetId() == 1) {
                    ly = -1;
                }
                if (world.GetBlock({p_origin.x * 16 + i, p_origin.y * 16 + j + 1}).GetId() == 1) {
                    ly = 1;
                }
                if(ly == 0 && lx == 0) {
                    tx = 4 + biome * 21;
                    ty = 0 + mod((g_y + g_x * 2), 4);
                } else {
                    tx = lx + 2 + biome * 21;
                    ty = ly + 1;
                }
            }
            if (id == 1) {
                tx = 0 + biome * 21;
                ty = 1 + mod((g_y + g_x * 2), 4);
            }

            quad[0].position = sf::Vector2f(i * kTileSize, j * kTileSize);
            quad[1].position = sf::Vector2f((i + 1) * kTileSize, j * kTileSize);
            quad[2].position = sf::Vector2f((i + 1) * kTileSize, (j + 1) * kTileSize);
            quad[3].position = sf::Vector2f(i * kTileSize, (j + 1) * kTileSize);

            quad[0].texCoords = sf::Vector2f(tx * kTextureSize, ty * kTextureSize);
            quad[1].texCoords = sf::Vector2f((tx + 1) * kTextureSize, ty * kTextureSize);
            quad[2].texCoords = sf::Vector2f((tx + 1) * kTextureSize, (ty + 1) * kTextureSize);
            quad[3].texCoords = sf::Vector2f(tx * kTextureSize, (ty + 1) * kTextureSize);
        }
    }
    setPosition(chunk.getOrigin().x * kChunkSize * kTileSize, chunk.getOrigin().y * kChunkSize * kTileSize);
}

void RenderChunk::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = tileset_ptr_;
    target.draw(vertices_, states);
}

void RenderEntities::Update(std::vector<Entity> &entities, TextureManager& texture_manager) {
    for(auto &entity : entities) {
        vertices_.setPrimitiveType(sf::Quads);
        vertices_.resize(kChunkSize * kChunkSize * 4);
        tileset_ptr_ = texture_manager.GetBlockTilesetPtr();
    }
}

void RenderEntities::draw(sf::RenderTarget& target, sf::RenderStates states) const{

};

void RenderWorld::Update(World& world, TextureManager& texture_manager){
    for(auto &chunk : world.chunks_) {
        render_chunks_[chunk.first].Update(chunk.second, texture_manager, world);
    }
}

void RenderWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(auto &render_chunk : render_chunks_) {
        render_chunk.second.draw(target, states);
    }
}