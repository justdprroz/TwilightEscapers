// Copyright 2021-2022 JustDprroz

// Define guards
#ifndef SOURCE_RENDER_HPP_
#define SOURCE_RENDER_HPP_

// SFML
#include <SFML/Graphics.hpp>

// Some base classes
#include <World.hpp>
#include <Utils.hpp>

// Main rendering constants
extern const int TEXTURE_SIZE;
extern const int TILE_SIZE;

// Stores all textures, layers, and everything related to them
class TextureManager {
public:
    // Constructors
    TextureManager();
    TextureManager(std::string assets_path);
    // Loaders
    void SetAssetsPath(std::string path);
    void LoadBlockTileset();
    void LoadEntityTileset();
    void LoadCharacters();
    // Getters
    sf::Texture* GetBlockTilesetPtr();
    sf::Texture* GetEntityTilesetPtr();
    sf::Texture* GetCharacterTexturePtr(int id);
private:
    // Path to directory with assets
    std::string assets_path_;
    // Atlases with textures
    sf::Texture block_tileset_;
    sf::Texture entity_tileset_;
    sf::Texture characters_textures_[16];
};

// Implements Custom Renderer for Chunks
class RenderChunk : public sf::Drawable, public sf::Transformable {
public:
    void Update(Chunk& chunk, TextureManager& texture_manager, World &world);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    sf::VertexArray vertices_;
    sf::Texture* tileset_ptr_;
};

// Implements Custom Renderer For Entities
class RenderEntity : public sf::Drawable, public sf::Transformable {
public:
    void Update(Entity* entity, TextureManager& texture_manager);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    sf::VertexArray vertices_;
    sf::Texture* tileset_ptr_;
};

// Main Renderer
class RenderWorld : public sf::Drawable, public sf::Transformable {
public:
    void Update(World& world, TextureManager& texture_manager);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    std::map<std::pair<int, int>, RenderChunk> render_chunks_;
    std::vector<RenderEntity> render_entities_;
};

#endif