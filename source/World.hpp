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

class Entity {
public:
    // Constructors and Inizializers
    Entity();
    Entity(int p_id) : m_id(p_id) {
        std::cout << m_id << '\n';
    };
    // Position and state managment
    void SetPosition(sf::Vector2f p_pos) {
        m_position = p_pos;
    }
    sf::Vector2f GetPosition() {
        return m_position;
    }
    int GetId(){
        return m_id;
    }
    // Update
    void Update(float p_tickTime) {
    }
protected:
    sf::Vector2f m_position;
    int m_id;
};

class Character : public Entity {
public:
    Character() : Entity(0) {};
    void HandleInput(sf::Event){
        // TODO:
    }
    std::string GetName() {
        return m_name;
    }
private:
    std::string m_name;
};

class Block {
public:
    Block() : m_id(0), m_biome(0){};
    Block(int p_id) : m_id(p_id), m_biome(0) {};
    Block(int p_id, int p_biome) : m_id(p_id), m_biome(p_biome) {};
    int GetId(){
        return m_id;
    }
    int GetBiome(){
        return m_biome;
    }
private:
    int m_id;
    int m_biome;
};

class Chunk {
public:
    Chunk() = default;
    Chunk(sf::Vector2i p_origin) : m_origin(p_origin) {};
    // Blocks
    Block GetBlock(sf::Vector2i p_pos) {
        return m_blocks[p_pos.x][p_pos.y];
    }
    void PlaceBlock(sf::Vector2i p_pos, const Block &p_block) {
        m_blocks[p_pos.x][p_pos.y] = p_block;
    }
    sf::Vector2i getOrigin() {
        return m_origin;
    }
    // Serialization & Deserialization
    void SaveChunk(std::string p_path) {
        std::ofstream chunkfile;
        chunkfile.open(p_path);
        if (!chunkfile.is_open()) {
            std::cout << "Failed open " << p_path << '\n';
            return;
        }
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                int l_id = m_blocks[i][j].GetId();
                chunkfile.write(reinterpret_cast<char *>(&l_id), sizeof(l_id));
            }
        }
        chunkfile.close();
    }
    void LoadChunk(std::string p_path) {
        std::ifstream chunkfile;
        chunkfile.open(p_path);
        if (!chunkfile.is_open()) {
            std::cout << "Failed open " << p_path << '\n';
            return;
        }
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                int l_id;
                chunkfile.read(reinterpret_cast<char *>(&l_id), sizeof(l_id));
                m_blocks[i][j] = { l_id };
            }
        }
        chunkfile.close();
    }
    void SetGenerated(){
        m_generated = true;
    };
    void SetGenerated(bool p_state){
        m_generated = p_state;
    };
    bool IsGenerated(){
        return m_generated;
    }
private:
    sf::Vector2i m_origin;
    bool m_generated = false;
    std::array<std::array<Block, 16>, 16> m_blocks;
    friend class RenderChunk;
    friend class Renderer;
};

class World {
public:
    // Constructors
    World(){};
    World(int p_seed) : m_seed{p_seed}{};
    void SetSeed(int p_seed) {
        m_seed = p_seed;
    }
    std::vector<Entity>& GetEntities() {
        return m_entities;
    }
    void SummonEntity(Entity p_entity){
        m_entities.push_back(p_entity);
    }
    // Blocks FIXME: math shit
    Block GetBlock(sf::Vector2i p_pos) {
        return m_chunks[{div<int>(p_pos.x, 16),div<int>(p_pos.y, 16)}].GetBlock({mod<int>(p_pos.x, 16),mod<int>(p_pos.y, 16)});
    }
    void PlaceBlock(sf::Vector2i p_pos, const Block &p_block) {
        m_chunks[{div<int>(p_pos.x, 16),div<int>(p_pos.y, 16)}].PlaceBlock({mod<int>(p_pos.x, 16),mod<int>(p_pos.y, 16)}, p_block);
    }
    // Chunks
    bool IsChunkExist(sf::Vector2i p_coords) {
        return m_chunks.find({p_coords.x, p_coords.y}) != m_chunks.end();
    }
    Chunk* GetChunk(sf::Vector2i p_coords) {
        if (!IsChunkExist({p_coords.x, p_coords.y})) {
            PlaceChunk(p_coords);
            GenerateChunk(p_coords);
        }
        return &m_chunks[{p_coords.x, p_coords.y}];
    }
    void PlaceChunk(sf::Vector2i p_coords) {
        if (IsChunkExist(p_coords)) return;
        m_chunks[{p_coords.x, p_coords.y}] = Chunk(p_coords);
    }
    void LoadChunks(std::string p_path) {
        std::ifstream chunkslist;
        chunkslist.open(p_path + "/chunkslist.tem");
        if (!chunkslist.is_open()) {
            std::cout << "Failed open " << p_path + "/chunkslist.tem" << '\n';
            return;
        }
        int lcx, lcy;
        while (!chunkslist.eof()) {
            chunkslist.read(reinterpret_cast<char *>(&lcx), sizeof(lcx));
            chunkslist.read(reinterpret_cast<char *>(&lcy), sizeof(lcy));
            m_chunks[{lcx, lcy}] = Chunk({lcx, lcy});
            m_chunks[{lcx, lcy}].LoadChunk(p_path + "/chunk_" + std::to_string(lcx) + "_" + std::to_string(lcy) + ".tem");
            // m_chunks[{lcx, lcy}].generated = true;
        }
        chunkslist.close();
    }
    void SaveChunks(std::string p_path) {
        std::ofstream chunkslist;
        chunkslist.open(p_path + "/chunkslist.tem");
        if (!chunkslist.is_open()) {
            std::cout << "Failed open " << p_path + "/chunkslist.tem" << '\n';
            return;
        }
        int lcx, lcy;
        for (std::map<std::pair<int, int>, Chunk>::iterator it = m_chunks.begin(); it != m_chunks.end(); ++it) {
            lcx = it->first.first;
            lcy = it->first.second;
            chunkslist.write(reinterpret_cast<char *>(&lcx), sizeof(lcx));
            chunkslist.write(reinterpret_cast<char *>(&lcy), sizeof(lcy));
            it->second.SaveChunk(p_path + "/chunk_" + std::to_string(lcx) + "_" + std::to_string(lcy) + ".tem");
        }
        chunkslist.close();
    }
    void GenerateChunk(sf::Vector2i p_coords) {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                float noiseHeightValue = m_noiseHeight.GetNoise(static_cast<float>(i + p_coords.x * 16), static_cast<float>(j + p_coords.y * 16));
                int blockId = static_cast<int>(LinearInterpolation(noiseHeightValue, -1.f, 1.0f, 0.0f, 2.0f));
                float noiseBiomeValue = m_noiseBiome.GetNoise(static_cast<float>(i + p_coords.x * 16), static_cast<float>(j + p_coords.y * 16));
                int biomeId = static_cast<int>(LinearInterpolation(noiseBiomeValue, -1.f, 1.0f, 0.0f, 3.0f));
                m_chunks[{p_coords.x, p_coords.y}].PlaceBlock({i, j}, {blockId, biomeId});
            }
        }
        m_chunks[{p_coords.x, p_coords.y}].SetGenerated();
    }
    void NoiseInit() {
        m_noiseHeight.SetSeed(m_seed);
        m_noiseHeight.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
        m_noiseHeight.SetFrequency(0.02);
        m_noiseHeight.SetFractalType(FastNoiseLite::FractalType_FBm);
        m_noiseHeight.SetFractalOctaves(3);

        m_noiseBiome.SetSeed(m_seed);
        m_noiseBiome.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
        m_noiseBiome.SetFrequency(0.02);
        m_noiseBiome.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Hybrid);
        m_noiseBiome.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
    }
private:
    // Methods
    // Members
    std::map<std::pair<int, int>, Chunk> m_chunks;
    std::vector<Entity> m_entities;
    FastNoiseLite m_noiseHeight;
    FastNoiseLite m_noiseBiome;
    int m_seed;
    friend class RenderWorld;
};

#endif  // SOURCE_WORLD_HPP_
