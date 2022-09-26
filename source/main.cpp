// Copyright 2021-2022 JustDprroz

#include <iostream>
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <Render.hpp>
#include <utility>
#include <Utils.hpp>

// Global constants
const int TEXTURE_SIZE = 16;
const int TILE_SIZE = 32;//32

// Local constants
const int RENDER_DISTANCE = 8;

void Terminal() {

}

int main() {
    sf::Font debugFont;
    debugFont.loadFromFile("assets/fonts/CascadiaCode.ttf");
    
    // Some variables
    bool debug = false;

    // Some variables for window
    std::string title = "EscapeFromTwilight ";
    int winWidth = 1000;
    int winHeight = 1000;
    float scale = 1;
    float zoom = 1;

    int viewWidth = winWidth * scale;
    int viewHeight = winHeight * scale;

    // Main render View
    sf::View mainView;
    mainView.setSize(viewWidth, viewHeight);
    mainView.setCenter(0, 0);
    mainView.zoom(zoom);

    sf::View textView;
    textView.setSize(viewWidth, viewHeight);
    textView.setCenter(viewWidth / 2, viewHeight / 2);

    // Setting to explicitly disable AA
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;

    // Create main render Window
    sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), title, sf::Style::None, settings);
    window.setView(mainView);
    window.setFramerateLimit(256); // check when you want
    // window.setVerticalSyncEnabled(true); // check when you want
    // bool fullscreen = false; // check when you want
    window.setKeyRepeatEnabled(false);

    sf::RenderTexture renderTexture;
    renderTexture.create(winWidth, winHeight);

    // Clocks and time
    sf::Clock mainRenderClock;
    // sf::Clock mainTickClock;

    // World
    World main_world;
    main_world.SetSeed(420);
    main_world.NoiseInit();

    // main_world.LoadChunks("DebugWorldSave");

    // Character
    Character mainCharacter(0, {0.0f, 0.0f});
    main_world.SummonEntity(mainCharacter);

    // Rendering
    TextureManager texture_manager("assets");
    RenderWorld mainRenderWorld;

    // Debug gui
    sf::Text text;
    text.setFont(debugFont);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setOutlineColor(sf::Color::White);
    text.setOutlineThickness(1);

    sf::Shader sh;
    sh.loadFromFile("frag.glsl", sf::Shader::Fragment);

    while (window.isOpen()) {
        float lastframetime = mainRenderClock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            mainCharacter.HandleEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::Resized){

                std::cout << "Resize invoked\n";

                winWidth = event.size.width;
                winHeight = event.size.height;
                viewWidth = winWidth * scale;
                viewHeight = winHeight * scale;

                mainView.setSize(sf::Vector2f(viewWidth, viewHeight));
                mainView.setCenter(0, 0);
                mainView.zoom(zoom);

                textView.setSize(sf::Vector2f(viewWidth, viewHeight));
                textView.setCenter(viewWidth / 2, viewHeight / 2);
            }
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Q) {
                    if(event.key.control){
                        main_world.SaveChunks("DebugWorldSave");
                    }
                }
                if (event.key.code == sf::Keyboard::Equal) {
                    zoom *= 1.05;
                    mainView.zoom(1.05);
                }
                if (event.key.code == sf::Keyboard::Dash) {
                    zoom /= 1.05;
                    mainView.zoom(1 / 1.05);
                }
                if (event.key.code == sf::Keyboard::F3 || event.key.code == sf::Keyboard::BackSlash) {
                    debug = !debug;
                }
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    if (event.mouseWheelScroll.delta < 0) {
                        zoom *= 1.05;
                        mainView.zoom(1.05);
                    } else if (event.mouseWheelScroll.delta > 0) {
                        zoom /= 1.05;
                        mainView.zoom(1 / 1.05);
                    }
                }
            }
        }

        // Invoke Updates
        mainCharacter.Update(lastframetime);
        sf::Vector2f pos = mainCharacter.GetPosition();
        sf::Vector2f currentChunkPosition = { std::floor(pos.x / 16), std::floor(pos.y / 16) };

        for (int o_x = -RENDER_DISTANCE; o_x <= RENDER_DISTANCE; o_x++) {
            for (int o_y = -RENDER_DISTANCE; o_y <= RENDER_DISTANCE; o_y++) {
                sf::Vector2i currentChunk = sf::Vector2i(currentChunkPosition) + sf::Vector2i(o_x, o_y);
                if (!main_world.IsChunkExist(currentChunk)) {
                    main_world.PlaceChunk(currentChunk);
                    main_world.GenerateChunk(currentChunk);
                }
            }
        }

        // Clear Window
        window.clear(sf::Color::Black);

        // Set positioned view
        window.setView(mainView);

        // Draw game staff
        mainRenderWorld.Update(main_world, texture_manager); 
        mainRenderWorld.setPosition(sf::Vector2f(-pos.x * TILE_SIZE, -pos.y * TILE_SIZE));

        renderTexture.setView(mainView);
        renderTexture.clear();
        renderTexture.draw(mainRenderWorld);

        // Draw debug staff

        if (debug) {
            sf::Vector2f manualRenderOffset = { -pos.x * TILE_SIZE, -pos.y * TILE_SIZE };
            sf::Vertex quad[] = {
                sf::Vertex(
                    sf::Vector2f(
                        manualRenderOffset.x + currentChunkPosition.x * 16 * TILE_SIZE,
                        manualRenderOffset.y + currentChunkPosition.y * 16 * TILE_SIZE
                    ),
                    sf::Color::Red
                ),
                sf::Vertex(
                    sf::Vector2f(
                        manualRenderOffset.x + (currentChunkPosition.x + 1) * 16 * TILE_SIZE,
                        manualRenderOffset.y + currentChunkPosition.y * 16 * TILE_SIZE
                    ),
                    sf::Color::Blue
                ),
                sf::Vertex(
                    sf::Vector2f(
                        manualRenderOffset.x + (currentChunkPosition.x + 1) * 16 * TILE_SIZE,
                        manualRenderOffset.y + (currentChunkPosition.y + 1) * 16 * TILE_SIZE
                    ),
                    sf::Color::Red
                ),
                sf::Vertex(
                    sf::Vector2f(
                        manualRenderOffset.x + currentChunkPosition.x * 16 * TILE_SIZE,
                        manualRenderOffset.y + (currentChunkPosition.y + 1) * 16 * TILE_SIZE
                    ),
                    sf::Color::Blue
                ),
                sf::Vertex(
                    sf::Vector2f(
                        manualRenderOffset.x + currentChunkPosition.x * 16 * TILE_SIZE,
                        manualRenderOffset.y + currentChunkPosition.y * 16 * TILE_SIZE
                    ),
                    sf::Color::Red
                ),
            };
            renderTexture.draw(quad, 5, sf::LineStrip);
        }
        renderTexture.display();

        sf::Sprite sprite;
        sprite.setTexture(renderTexture.getTexture());
        sprite.setPosition({-winWidth / 2, -winHeight / 2});

        window.draw(sprite, &sh);

        // Set inteface view
        window.setView(textView);

        // Draw game staff

        // Draw debug staff
        if (debug) {
            text.setString(
                std::to_string(1.0 / lastframetime) + '\n' +
                std::to_string(mainCharacter.GetPosition().x) + " " + std::to_string(mainCharacter.GetPosition().y)
            );

            window.draw(text);
        }

        // Display everything drawn
        window.display();
    }

    // mainWorld.SaveChunks("World1");
    return 0;
}