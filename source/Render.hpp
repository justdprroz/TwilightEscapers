// Copyright 2021-2022 JustDprroz

#ifndef SOURCE_RENDER_HPP_
#define SOURCE_RENDER_HPP_

#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <Utils.hpp>

const int kTextureSize = 16;
const int kTileSize = 48;

class TextureManager {
public:
    TextureManager() = default;
    TextureManager(std::string assets_path);
    void SetAssetsPath(std::string path);
    void LoadBlockTileset();
    void LoadEntityTileset();
    sf::Texture* GetBlockTilesetPtr();
    sf::Texture* GetEntityTilesetPtr();
private:
    std::string assets_path_;
    sf::Texture block_tileset_;
    sf::Texture entity_tileset_;
};

class RenderChunk : public sf::Drawable, public sf::Transformable {
public:
    void Update(Chunk& chunk, TextureManager& texture_manager, World &world);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    sf::VertexArray vertices_;
    sf::Texture* tileset_ptr_;
};

class RenderEntities : public sf::Drawable, public sf::Transformable {
public:
    void Update(std::vector<Entity*> &entities, TextureManager& texture_manager);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    sf::VertexArray vertices_;
    sf::Texture* tileset_ptr_;
};

class RenderWorld : public sf::Drawable, public sf::Transformable {
public:
    void Update(World& world, TextureManager& texture_manager);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    std::map<std::pair<int, int>, RenderChunk> render_chunks_;
    RenderEntities render_entities_;
};

#endif