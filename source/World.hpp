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
#include <climits>
#include <SFML/Window/Event.hpp>

const int kChunkSize = 16;

class Entity {
public:
    Entity();
    Entity(int p_id, sf::Vector2f pos);
    void SetPosition(sf::Vector2f position);
    void SetVelocity(sf::Vector2f velocity);
    int GetId();
    int GetType();
    int GetHeading();
    int GetState();
    int GetAnimFrame();
    int GetSpeed();
    sf::Vector2f GetPosition();
    sf::Vector2f GetVelocity();
    void Update(float tick_time);
protected:
    sf::Vector2f position_;
    sf::Vector2f velocity_;
    float speed_;
    int id_;
    int type_;
    int heading_;
    int state_;
    int anim_frame_;
};

class Character : public Entity {
public:
    Character();
    Character(int id, sf::Vector2f pos);
    void HandleEvent(sf::Event &event);
    void Update(float tick_time);
private:
    std::string name_;
    bool pressed_[4] = {false, false, false, false};
};

class Block {
public:
    Block();
    Block(int);
    Block(int, int);
    int GetId();
    int GetBiome();
private:
    int id_;
    int biome_;
};

class Chunk {
public:
    Chunk();
    Chunk(sf::Vector2i origin);
    Block GetBlock(sf::Vector2i position);
    void PlaceBlock(sf::Vector2i position, const Block &block);
    sf::Vector2i GetOrigin();
    void SaveChunk(std::string file_path);
    void LoadChunk(std::string file_path);
    void SetGenerated();
    void SetGenerated(bool state);
    bool IsGenerated();
private:
    sf::Vector2i origin_;
    bool is_generated_;
    std::array<std::array<Block, kChunkSize>, kChunkSize> blocks_;
    friend class RenderChunk;
    friend class Renderer;
};

class World {
public:
    World();
    World(int seed);
    void SetSeed(int seed);
    std::vector<Entity*>& GetEntities();
    void SummonEntity(Entity &entity);
    Block GetBlock(sf::Vector2i position);
    void PlaceBlock(sf::Vector2i position, Block &block);
    bool IsChunkExist(sf::Vector2i position);
    Chunk* GetChunk(sf::Vector2i position);
    void PlaceChunk(sf::Vector2i position);
    void LoadChunks(std::string file_path);
    void SaveChunks(std::string file_path);
    void GenerateChunk(sf::Vector2i position);
    void NoiseInit();
private:
    std::map<std::pair<int, int>, Chunk> chunks_;
    std::vector<Entity*> entities_;
    FastNoiseLite noise_block_;
    FastNoiseLite noise_biome_;
    int seed_;
    friend class RenderWorld;
};

#endif  // SOURCE_WORLD_HPP_
