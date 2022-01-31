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

void RenderChunk::Update(Chunk& chunk, TextureManager& texture_manager) {
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
                tx = 4 + biome * 21;
                ty = 0 + mod((g_y + g_x * 2), 4);
            }
            if (id == 1) {
                tx = 0 + biome * 21;
                ty = 1 + mod((g_y + g_x * 2), 4);
            }

            quad[0].position = sf::Vector2f(i * kTextureSize, j * kTextureSize);
            quad[1].position = sf::Vector2f((i + 1) * kTextureSize, j * kTextureSize);
            quad[2].position = sf::Vector2f((i + 1) * kTextureSize, (j + 1) * kTextureSize);
            quad[3].position = sf::Vector2f(i * kTextureSize, (j + 1) * kTextureSize);

            quad[0].texCoords = sf::Vector2f(tx * kTextureSize, ty * kTextureSize);
            quad[1].texCoords = sf::Vector2f((tx + 1) * kTextureSize - 1, ty * kTextureSize);
            quad[2].texCoords = sf::Vector2f((tx + 1) * kTextureSize - 1, (ty + 1) * kTextureSize - 1);
            quad[3].texCoords = sf::Vector2f(tx * kTextureSize, (ty + 1) * kTextureSize - 1);
        }
    }
    setPosition(chunk.getOrigin().x * kChunkSize * kTextureSize, chunk.getOrigin().y * kChunkSize * kTextureSize);
}

void RenderChunk::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = tileset_ptr_;
    target.draw(vertices_, states);
}

void RenderWorld::Update(World& world, TextureManager& texture_manager){
    for(auto chunk : world.chunks_) {
        render_chunks_[chunk.first].Update(chunk.second, texture_manager);
    }
}

void RenderWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(auto render_chunk : render_chunks_) {
        render_chunk.second.draw(target, states);
    }
}