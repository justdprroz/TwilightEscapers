// Copyright 2021 JustDprroz
#ifndef SOURCE_WORLD_HPP_
#define SOURCE_WORLD_HPP_

#include <cstdlib>
#include <ctime>
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
    void SetPosition(sf::Vector2f p_pos) {
        m_position = p_pos;
    }
    sf::Vector2f GetPosition() {
        return m_position;
    }

private:
    sf::Vector2f m_position;
};

struct Block {
    int m_id;
};

struct Chunk {
public:
    Chunk() = default;
    // Blocks
    Block* GetBlock(sf::Vector2i p_pos) {
        return &m_blocks[p_pos.x][p_pos.y];
    }
    void SetBlock(sf::Vector2i p_pos, const Block &p_block) {
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
                int l_id = m_blocks[i][j].m_id;
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
    sf::Vector2i m_origin;
    std::array<std::array<Block, 16>, 16> m_blocks;
};
class World {
public:
    World() = default;
    void Init() {
        m_noise.SetSeed(m_seed);
        m_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
        m_noise.SetFrequency(0.02);
        m_noise.SetFractalType(FastNoiseLite::FractalType_FBm);
        m_noise.SetFractalOctaves(3);
    }
    void SetSeed(int p_seed) {
        m_seed = p_seed;
    }
    Block* GetBlock(sf::Vector2i p_pos) {
        return GetChunk({p_pos.x / 16, p_pos.y / 16})->GetBlock({p_pos.x % 16, p_pos.y % 16});
    }
    void SetBlock(sf::Vector2i p_pos, const Block &p_block) {
        m_chunks[{p_pos.x / 16, p_pos.y / 16}].SetBlock({p_pos.x % 16, p_pos.y % 16}, p_block);
    }
    bool IsChunkExist(sf::Vector2i p_coords) {
        return m_chunks.find({p_coords.x, p_coords.y}) != m_chunks.end();
    }
    Chunk* GetChunk(sf::Vector2i p_coords) {
        if (!IsChunkExist({p_coords.x, p_coords.y})) {
            SetChunk(p_coords);
            GenerateChunk(p_coords);
        }
        return &m_chunks[{p_coords.x, p_coords.y}];
    }
    void SetChunk(sf::Vector2i p_coords) {
        if (IsChunkExist(p_coords)) return;
        m_chunks[{p_coords.x, p_coords.y}] = Chunk();
        m_chunks[{p_coords.x, p_coords.y}].m_origin = p_coords;
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
            m_chunks[{lcx, lcy}] = Chunk();
            m_chunks[{lcx, lcy}].m_origin = {lcx, lcy};
            m_chunks[{lcx, lcy}].LoadChunk(p_path + "/chunk_" + std::to_string(lcx) + "_" + std::to_string(lcy) + ".tem");
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
        m_chunks[{p_coords.x, p_coords.y}].m_origin = p_coords;
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                float noiseValue = m_noise.GetNoise(static_cast<float>(i + p_coords.x * 16), static_cast<float>(j + p_coords.y * 16));
                int blockId = static_cast<int>(LinearInterpolation(noiseValue, -1.f, 1.0f, 0.0f, 2.0f));
                m_chunks[{p_coords.x, p_coords.y}].m_blocks[i][j] = {blockId};
            }
        }
    }

private:
    FastNoiseLite m_noise;
    int m_seed;
    std::map<std::pair<int, int>, Chunk> m_chunks;
    std::vector<Entity> m_entities;
};

#endif  // SOURCE_WORLD_HPP_
