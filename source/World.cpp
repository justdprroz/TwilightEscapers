// Copyright 2021-2022 JustDprroz

#include "World.hpp"

Entity::Entity() {
    id_ = 0;
    type_ = 0;
    position_ = {0.0f, 0.0f};
    velocity_ = {0.0f, 0.0f};
    speed_ = 10;
    heading_ = 0;
}

Entity::Entity(int p_id, sf::Vector2f pos) : Entity() {
    id_ = p_id;
    position_ = pos;
}

void Entity::SetId(int id) {
    id_ = id;
}

int Entity::GetId() {
    return id_;
}

void Entity::SetPosition(sf::Vector2f position) {
    position_ = position;
}

sf::Vector2f Entity::GetPosition() {
    return position_;
}

void Entity::SetVelocity(sf::Vector2f velocity) {
    velocity_ = velocity;
}

sf::Vector2f Entity::GetVelocity() {
    return velocity_;
}

void Entity::Update(float tick_time) {
    position_ += velocity_ * tick_time;
}

Character::Character() : Entity() {

};

Character::Character(int id, sf::Vector2f pos) : Entity(id, pos) {

}

void Character::HandleEvent(sf::Event &event) {
    if (event.KeyPressed) {
        if (event.type == sf::Event::KeyPressed){
            if (event.key.code == sf::Keyboard::W){
                pressed_[0] = true;
            }
            if (event.key.code == sf::Keyboard::D){
                pressed_[1] = true;
            }
            if (event.key.code == sf::Keyboard::S){
                pressed_[2] = true;
            }
            if (event.key.code == sf::Keyboard::A){
                pressed_[3] = true;
            }
        }
        if (event.type == sf::Event::KeyReleased){
            if (event.key.code == sf::Keyboard::W){
                pressed_[0] = false;
            }
            if (event.key.code == sf::Keyboard::D){
                pressed_[1] = false;
            }
            if (event.key.code == sf::Keyboard::S){
                pressed_[2] = false;
            }
            if (event.key.code == sf::Keyboard::A){
                pressed_[3] = false;
            }
        }
    }
}

void Character::Update(float tick_time) {
    velocity_.x = ((int)pressed_[1] - (int)pressed_[3]) * speed_;
    velocity_.y = ((int)pressed_[2] - (int)pressed_[0]) * speed_;
    Entity::Update(tick_time);
}

Block::Block() : id_(0), biome_(0) {

}

Block::Block(int id) : id_(id), biome_(0) {

}

Block::Block(int id, int biome) : id_(id), biome_(biome) {

}

int Block::GetId() {
    return id_;
}

int Block::GetBiome() {
    return biome_;
}

Chunk::Chunk() {

};

Chunk::Chunk(sf::Vector2i origin) : origin_(origin){

}

Block Chunk::GetBlock(sf::Vector2i position) {
    return blocks_[position.x][position.y];
}

void Chunk::PlaceBlock(sf::Vector2i position, const Block& block) {
    blocks_[position.x][position.y] = block;
}

sf::Vector2i Chunk::GetOrigin() {
    return origin_;
}

void Chunk::SetGenerated() {
    is_generated_ = true;
}

void Chunk::SetGenerated(bool state) {
    is_generated_ = state;
}

bool Chunk::IsGenerated() {
    return is_generated_;
}

// FIXME: normal serialization
void Chunk::SaveChunk(std::string file_path) {
    std::ofstream chunkfile;
    chunkfile.open(file_path);
    if (!chunkfile.is_open()) {
        std::cout << "Failed open " << file_path << '\n';
        return;
    }
    for (int i = 0; i < kChunkSize; i++) {
        for (int j = 0; j < kChunkSize; j++) {
            int l_id = blocks_[i][j].GetId();
            chunkfile.write(reinterpret_cast<char *>(&l_id), sizeof(l_id));
        }
    }
    chunkfile.close();
}

// FIXME: normal deserialization
void Chunk::LoadChunk(std::string file_path) {
    std::ifstream chunkfile;
    chunkfile.open(file_path);
    if (!chunkfile.is_open()) {
        std::cout << "Failed open " << file_path << '\n';
        return;
    }
    for (int i = 0; i < kChunkSize; i++) {
        for (int j = 0; j < kChunkSize; j++) {
            int l_id;
            chunkfile.read(reinterpret_cast<char *>(&l_id), sizeof(l_id));
            blocks_[i][j] = { l_id };
        }
    }
    chunkfile.close();
}

World::World() {

}

World::World(int seed) : seed_(seed) {

}

void World::SetSeed(int seed) {
    seed_ = seed;
}

std::vector<Entity*>& World::GetEntities() {
    return entities_;
}

// TODO: add entity management system
void World::SummonEntity(Entity &entity){
    entities_.push_back(&entity);
}

Block World::GetBlock(sf::Vector2i position) {
    sf::Vector2i chunk_position = {div<int>(position.x, kChunkSize),div<int>(position.y, kChunkSize)};
    if (!IsChunkExist(chunk_position)) return {INT_MIN};
    return chunks_[{chunk_position.x, chunk_position.y}].GetBlock({mod<int>(position.x, kChunkSize),mod<int>(position.y, kChunkSize)});
}

void World::PlaceBlock(sf::Vector2i position, Block &block) {
    chunks_[{div<int>(position.x, kChunkSize),div<int>(position.y, kChunkSize)}].PlaceBlock({mod<int>(position.x, kChunkSize),mod<int>(position.y, kChunkSize)}, block);
}

bool World::IsChunkExist(sf::Vector2i position) {
    return chunks_.find({position.x, position.y}) != chunks_.end();
}

Chunk* World::GetChunk(sf::Vector2i position) {
    if (!IsChunkExist({position.x, position.y})) {
        PlaceChunk(position);
        GenerateChunk(position);
    }
    return &chunks_[{position.x, position.y}];
}

void World::PlaceChunk(sf::Vector2i position) {
    if (IsChunkExist(position)) return;
    chunks_[{position.x, position.y}] = Chunk(position);
}

// FIXME: normal serialization
void World::SaveChunks(std::string file_path) {
    std::ofstream chunkslist;
    chunkslist.open(file_path + "/chunkslist.tem");
    if (!chunkslist.is_open()) {
        std::cout << "Failed open " << file_path + "/chunkslist.tem" << '\n';
        return;
    }
    int lcx, lcy;
    for (std::map<std::pair<int, int>, Chunk>::iterator it = chunks_.begin(); it != chunks_.end(); ++it) {
        lcx = it->first.first;
        lcy = it->first.second;
        chunkslist.write(reinterpret_cast<char *>(&lcx), sizeof(lcx));
        chunkslist.write(reinterpret_cast<char *>(&lcy), sizeof(lcy));
        it->second.SaveChunk(file_path + "/chunk_" + std::to_string(lcx) + "_" + std::to_string(lcy) + ".tem");
    }
    chunkslist.close();
}

// FIXME: normal deserialization
void World::LoadChunks(std::string file_path) {
    std::ifstream chunkslist;
    chunkslist.open(file_path + "/chunkslist.tem");
    if (!chunkslist.is_open()) {
        std::cout << "Failed open " << file_path + "/chunkslist.tem" << '\n';
        return;
    }
    int lcx, lcy;
    while (!chunkslist.eof()) {
        chunkslist.read(reinterpret_cast<char *>(&lcx), sizeof(lcx));
        chunkslist.read(reinterpret_cast<char *>(&lcy), sizeof(lcy));
        chunks_[{lcx, lcy}] = Chunk({lcx, lcy});
        chunks_[{lcx, lcy}].LoadChunk(file_path + "/chunk_" + std::to_string(lcx) + "_" + std::to_string(lcy) + ".tem");
    }
    chunkslist.close();
}

// TODO: Great world generation
void World::GenerateChunk(sf::Vector2i position) {
    for (int i = 0; i < kChunkSize; i++) {
        for (int j = 0; j < kChunkSize; j++) {
            float noiseHeightValue = noise_block_.GetNoise(static_cast<float>(i + position.x * kChunkSize), static_cast<float>(j + position.y * kChunkSize));
            int blockId = static_cast<int>(LinearInterpolation(noiseHeightValue, -1.f, 1.0f, 0.0f, 2.0f));
            float noiseBiomeValue = noise_biome_.GetNoise(static_cast<float>(i + position.x * kChunkSize), static_cast<float>(j + position.y * kChunkSize));
            float biomeInterpolated = LinearInterpolation(noiseBiomeValue, -1.f, 1.0f, 0.0f, 3.0f);
            int biomeId = 0;
            if (InRange(biomeInterpolated, 0.0f, 0.5f)) {
                biomeId = 0;
            } else if (InRange(biomeInterpolated, 0.5f, 0.9f)) {
                biomeId = 1;
            } else if (InRange(biomeInterpolated, 0.9f, 1.1f)) {
                biomeId = 2;
            }
            chunks_[{position.x, position.y}].PlaceBlock({i, j}, {blockId, biomeId});
        }
    }
    chunks_[{position.x, position.y}].SetGenerated();
}

void World::NoiseInit() {
    noise_block_.SetSeed(seed_);
    noise_block_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    noise_block_.SetFrequency(0.02);
    noise_block_.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise_block_.SetFractalOctaves(3);

    noise_biome_.SetSeed(seed_);
    noise_biome_.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    noise_biome_.SetFrequency(0.02);
    noise_biome_.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Hybrid);
    noise_biome_.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
}