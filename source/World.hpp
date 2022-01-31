// Copyright 2021-2022 JustDprroz

#ifndef SOURCE_WORLD_HPP_
#define SOURCE_WORLD_HPP_

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <SFML/System/Vector2.hpp>
#include <external/FastNoiseLite.h>
#include <Utils.hpp>

const int kChunkSize = 16;

class Entity {
public:
    // Constructors and Inizializers
    Entity();
    Entity(int p_id) : id_(p_id) {
        std::cout << id_ << '\n';
    };
    // Position and state managment
    void SetPosition(sf::Vector2f p_pos) {
        position_ = p_pos;
    }
    sf::Vector2f GetPosition() {
        return position_;
    }
    int GetId(){
        return id_;
    }
    // Update
    void Update(float p_tickTime) {
    }
protected:
    sf::Vector2f position_;
    int id_;
};

class Character : public Entity {
public:
    Character() : Entity(0) {};
    void HandleInput(sf::Event){
        // TODO:
    }
    std::string GetName() {
        return name_;
    }
private:
    std::string name_;
};

class Block {
public:
    Block() : id_(0), biome_(0){};
    Block(int p_id) : id_(p_id), biome_(0) {};
    Block(int p_id, int p_biome) : id_(p_id), biome_(p_biome) {};
    int GetId(){
        return id_;
    }
    int GetBiome(){
        return biome_;
    }
private:
    int id_;
    int biome_;
};

class Chunk {
public:
    Chunk() = default;
    Chunk(sf::Vector2i origin) : origin_(origin) {};
    // Blocks
    Block GetBlock(sf::Vector2i position) {
        return blocks_[position.x][position.y];
    }
    void PlaceBlock(sf::Vector2i position, const Block &block) {
        blocks_[position.x][position.y] = block;
    }
    sf::Vector2i getOrigin() {
        return origin_;
    }
    // Serialization & Deserialization
    void SaveChunk(std::string file_path) {
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
    void LoadChunk(std::string file_path) {
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
    void SetGenerated(){
        is_generated_ = true;
    };
    void SetGenerated(bool state){
        is_generated_ = state;
    };
    bool IsGenerated(){
        return is_generated_;
    }
private:
    sf::Vector2i origin_;
    bool is_generated_ = false;
    std::array<std::array<Block, kChunkSize>, kChunkSize> blocks_;
    friend class RenderChunk;
    friend class Renderer;
};

class World {
public:
    // Constructors
    World(){};
    World(int seed) : seed_{seed}{};
    void SetSeed(int seed) {
        seed_ = seed;
    }
    std::vector<Entity>& GetEntities() {
        return entities_;
    }
    void SummonEntity(Entity entity){
        entities_.push_back(entity);
    }
    // Blocks
    Block GetBlock(sf::Vector2i position) {
        return chunks_[{div<int>(position.x, kChunkSize),div<int>(position.y, kChunkSize)}].GetBlock({mod<int>(position.x, kChunkSize),mod<int>(position.y, kChunkSize)});
    }
    void PlaceBlock(sf::Vector2i position, const Block &block) {
        chunks_[{div<int>(position.x, kChunkSize),div<int>(position.y, kChunkSize)}].PlaceBlock({mod<int>(position.x, kChunkSize),mod<int>(position.y, kChunkSize)}, block);
    }
    // Chunks
    bool IsChunkExist(sf::Vector2i position) {
        return chunks_.find({position.x, position.y}) != chunks_.end();
    }
    Chunk* GetChunk(sf::Vector2i position) {
        if (!IsChunkExist({position.x, position.y})) {
            PlaceChunk(position);
            GenerateChunk(position);
        }
        return &chunks_[{position.x, position.y}];
    }
    void PlaceChunk(sf::Vector2i position) {
        if (IsChunkExist(position)) return;
        chunks_[{position.x, position.y}] = Chunk(position);
    }
    void LoadChunks(std::string file_path) {
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
    void SaveChunks(std::string file_path) {
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
    void GenerateChunk(sf::Vector2i position) {
        for (int i = 0; i < kChunkSize; i++) {
            for (int j = 0; j < kChunkSize; j++) {
                float noiseHeightValue = noise_block_.GetNoise(static_cast<float>(i + position.x * kChunkSize), static_cast<float>(j + position.y * kChunkSize));
                int blockId = static_cast<int>(LinearInterpolation(noiseHeightValue, -1.f, 1.0f, 0.0f, 2.0f));
                float noiseBiomeValue = noise_biome_.GetNoise(static_cast<float>(i + position.x * kChunkSize), static_cast<float>(j + position.y * kChunkSize));
                int biomeId = static_cast<int>(LinearInterpolation(noiseBiomeValue, -1.f, 1.0f, 0.0f, 3.0f));
                chunks_[{position.x, position.y}].PlaceBlock({i, j}, {blockId, biomeId});
            }
        }
        chunks_[{position.x, position.y}].SetGenerated();
    }
    void NoiseInit() {
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
private:
    // Methods
    // Members
    std::map<std::pair<int, int>, Chunk> chunks_;
    std::vector<Entity> entities_;
    FastNoiseLite noise_block_;
    FastNoiseLite noise_biome_;
    int seed_;
    friend class RenderWorld;
};

#endif  // SOURCE_WORLD_HPP_
