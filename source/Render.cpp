// Copyright 2021-2022 JustDprroz

#include "Render.hpp"
#include <chrono>

TextureManager::TextureManager() {

}

TextureManager::TextureManager(std::string assets_path) {
    assets_path_ = assets_path;
    LoadBlockTileset();
    LoadEntityTileset();
    LoadCharacters();
}

void TextureManager::SetAssetsPath(std::string path) {
    assets_path_ = path;
}

void TextureManager::LoadBlockTileset() {
    block_tileset_.loadFromFile(assets_path_ + "/textures/tiles/tileset.png");
}

void TextureManager::LoadEntityTileset() {
    entity_tileset_.loadFromFile(assets_path_ + "/textures/entity/characters_tileset.png");
}

void TextureManager::LoadCharacters() {
    for(int i = 0; i < CHARACTERS_AMOUNT; i++){ 
        characters_textures_[i] = sf::Texture();
        characters_textures_[i].loadFromFile(assets_path_ + "/textures/entity/characters/" + std::to_string(0) + ".png");
    }
}

sf::Texture* TextureManager::GetBlockTilesetPtr() {
    return &block_tileset_;
}

sf::Texture* TextureManager::GetEntityTilesetPtr() {
    return &entity_tileset_;
}

sf::Texture* TextureManager::GetCharacterTexturePtr(int id) {
    return &characters_textures_[id];
}

void RenderChunk::Update(Chunk& chunk, TextureManager& texture_manager, World &world) {
    vertices_.setPrimitiveType(sf::Quads);
    vertices_.resize(kChunkSize * kChunkSize * 4);
    tileset_ptr_ = texture_manager.GetBlockTilesetPtr();
    sf::Vector2i p_origin = chunk.GetOrigin();
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
    setPosition(chunk.GetOrigin().x * kChunkSize * kTileSize, chunk.GetOrigin().y * kChunkSize * kTileSize);
}

void RenderChunk::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = tileset_ptr_;
    target.draw(vertices_, states);
}

void RenderEntity::Update(Entity* entity, TextureManager& texture_manager) {
    std::cout << "setting quad type" << '\n';
    vertices_.setPrimitiveType(sf::Quads);
    std::cout << "resizing " << entity << '\n';
    vertices_.resize(4);

    sf::Vertex* quad = &vertices_[0];
    sf::Vector2f pos = entity->GetPosition();

    int type = entity->GetType();
    int id = entity->GetId();
    int state = entity->GetState();
    int heading = entity->GetHeading();
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );
    int frame = ms.count() / 250 % 4;

    tileset_ptr_ = texture_manager.GetCharacterTexturePtr(id);

    int tx = 0;
    int ty = 0;

    if (state == 0) {
        ty = 0;
    }

    if (state == 1) {
        ty = 0 + frame;
    }

    if (heading == 0) {
        tx = 1;
    }
    if (heading == 1) {
        tx = 3;
    }
    if (heading == 2) {
        tx = 0;
    }
    if (heading == 3) {
        tx = 2;
    }

    quad[0].position = sf::Vector2f(pos.x * kTileSize, pos.y * kTileSize);
    quad[1].position = sf::Vector2f((pos.x + 1) * kTileSize, pos.y * kTileSize);
    quad[2].position = sf::Vector2f((pos.x + 1) * kTileSize, (pos.y + 1) * kTileSize);
    quad[3].position = sf::Vector2f(pos.x * kTileSize, (pos.y + 1) * kTileSize);

    quad[0].texCoords = sf::Vector2f(tx * kTextureSize, ty * kTextureSize);
    quad[1].texCoords = sf::Vector2f((tx + 1) * kTextureSize, ty * kTextureSize);
    quad[2].texCoords = sf::Vector2f((tx + 1) * kTextureSize, (ty + 1) * kTextureSize);
    quad[3].texCoords = sf::Vector2f(tx * kTextureSize, (ty + 1) * kTextureSize);

}

void RenderEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    states.transform *= getTransform();
    states.texture = tileset_ptr_;
    target.draw(vertices_, states);
};

void RenderEntities::Update(std::vector<Entity*> &entities, TextureManager& texture_manager) {
    for(int i = 0; i < entities.size(); i++) {
        vertices_.setPrimitiveType(sf::Quads);
        vertices_.resize(entities.size() * 4);
        tileset_ptr_ = texture_manager.GetEntityTilesetPtr();

        sf::Vertex* quad = &vertices_[i * 4];
        sf::Vector2f pos = entities[i]->GetPosition();

        int type = entities[i]->GetId();
        int id = entities[i]->GetId();

        quad[0].position = sf::Vector2f(pos.x * kTileSize, pos.y * kTileSize);
        quad[1].position = sf::Vector2f((pos.x + 1) * kTileSize, pos.y * kTileSize);
        quad[2].position = sf::Vector2f((pos.x + 1) * kTileSize, (pos.y + 1) * kTileSize);
        quad[3].position = sf::Vector2f(pos.x * kTileSize, (pos.y + 1) * kTileSize);

        quad[0].texCoords = sf::Vector2f(0, 0 + id * kTextureSize);
        quad[1].texCoords = sf::Vector2f(kTextureSize, 0 + id * kTextureSize);
        quad[2].texCoords = sf::Vector2f(kTextureSize, kTextureSize + id * kTextureSize);
        quad[3].texCoords = sf::Vector2f(0, kTextureSize + id * kTextureSize);
    }
}

void RenderEntities::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    states.transform *= getTransform();
    states.texture = tileset_ptr_;
    target.draw(vertices_, states);
};

void RenderWorld::Update(World& world, TextureManager& texture_manager){
    std::cout << "updating chunks" << '\n';
    for(auto &chunk : world.chunks_) {
        render_chunks_[chunk.first].Update(chunk.second, texture_manager, world);
    }
    std::cout << "updating entities" << '\n';
    for(int i = 0; i < world.entities_.size(); i++){
        if(i >= render_entities_.size()) render_entities_.push_back(RenderEntity());
        render_entities_[i].Update(world.entities_[i], texture_manager);
    }
}

void RenderWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(auto &render_chunk : render_chunks_) {
        render_chunk.second.draw(target, states);
    }
    for(int i = 0; i < render_entities_.size(); i++){
        render_entities_[i].draw(target, states);
    }
}