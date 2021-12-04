// Copyright 2021 JustDprroz

#ifndef SOURCE_WORLD_HPP_
#define SOURCE_WORLD_HPP_

#include <string>
#include <array>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <SFML/System/Vector2.hpp>

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

class Block {
public:
    Block(){
        m_id = 1;
    }
    Block(int p_id){
        m_id = p_id;
    }
    int GetId() {
        return m_id;
    }
private:
    int m_id;
};

class Chunk {
public:
    Block GetBlock(sf::Vector2i p_pos) {
        const sf::Vector2i localPos = p_pos - m_origin * 16;
        return m_blocks[localPos.x][localPos.y];
    }
    void SetBlock(sf::Vector2i p_pos, const Block &p_block) {
        const sf::Vector2i localPos = p_pos - m_origin * 16;
        m_blocks[localPos.x][localPos.y] = p_block;
    }
    void SaveChunk(std::string p_path){
        std::ofstream chunkfile;
        // chunkfile.open(p_path + "\\chunk_" + std::to_string(m_origin.x) + "_" + std::to_string(m_origin.y) + ".tem");
        chunkfile.open(p_path);
        for(int i = 0; i < 16; i++) {
            for(int j = 0; j < 16; j++) {
                int l_id = m_blocks[i][j].GetId();
                std::cout << "Saving block " << i << ' ' << j << ' ' << l_id << '\n';
                chunkfile.write((char *)&l_id, sizeof(l_id));
            }
        }
        chunkfile.close();
    }
    void LoadChunk(std::string p_path){
        std::ifstream chunkfile;
        chunkfile.open(p_path);
        for(int i = 0; i < 16; i++) {
            for(int j = 0; j < 16; j++) {
                int l_id;
                chunkfile.read((char *)&l_id, sizeof(l_id));
                std::cout << "reading block " << i << ' ' << j << ' ' << l_id << '\n';
                m_blocks[i][j] = Block(l_id);
            }
        }
        chunkfile.close();
    }
private:
    sf::Vector2i m_origin;
    std::array<std::array<Block, 16>, 16> m_blocks;
};

class World {
public:
    Block GetBlock(sf::Vector2i p_pos) {
        m_chunks[{p_pos.x / 16, p_pos.y / 16}].GetBlock(p_pos);
    }
    void SetBlock(sf::Vector2i p_pos, const Block &p_block) {
        m_chunks[{p_pos.x / 16, p_pos.y / 16}]
            .SetBlock(p_pos, p_block);
    }
    bool IsExist(sf::Vector2i p_coords) {
        return m_chunks.find({p_coords.x, p_coords.y}) != m_chunks.end();
    }
    Chunk GetChunk(sf::Vector2i p_coords){
        return m_chunks[{p_coords.x, p_coords.y}];
    }
    void SetChunk(sf::Vector2i p_coords, Chunk &p_chunk){
        m_chunks[{p_coords.x, p_coords.y}] = p_chunk;
    }
private:
    std::map<std::pair<int, int>, Chunk> m_chunks;
    std::vector<Entity> m_entities;
};

#endif  // SOURCE_WORLD_HPP_
