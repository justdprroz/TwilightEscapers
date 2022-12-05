// Copyright 2021-2022 JustDprroz

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Sequoia.hpp>
#include <utility>
#include <Utils.hpp>
#include <algorithm>
#include <csignal>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>

std::ifstream f("config.json");
nlohmann::json data = nlohmann::json::parse(f);

// Global constants
const int TEXTURE_SIZE = data["TEXTURE_SIZE"];
const int TILE_SIZE = data["TILE_SIZE"]; // 32

// Local constants
int RENDER_DISTANCE = data["RENDER_DISTANCE"];

void Terminal()
{
}

int main()
{

    // std::cout << data << '\n';

    sf::Font debugFont;
    debugFont.loadFromFile("assets/fonts/CascadiaCode.ttf");

    // Some variables
    bool debug = false;

    // Some variables for window
    std::string title = data["WINDOW_TITLE"];
    int winWidth = 1000;
    int winHeight = 1000;
    float scale = 1;
    float zoom = data["ZOOM"];

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
    settings.antialiasingLevel = 16;

    // Create main render Window
    sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), title, sf::Style::Default);
    window.setView(mainView);
    // window.setFramerateLimit(256); // check when you want
    // window.setVerticalSyncEnabled(true); // check when you want
    // bool fullscreen = true; // check when you want
    window.setKeyRepeatEnabled(false);

    sf::RenderTexture renderTexture;
    renderTexture.create(winWidth, winHeight);

    // Clocks and time
    sf::Clock mainRenderClock;
    // sf::Clock mainTickClock;

    // World
    World main_world;
    main_world.SetSeed(420);
    // main_world.NoiseInit();

    // main_world.LoadChunks("DebugWorldSave");

    // Player
    Player mainCharacter(1, {0.0f, 0.0f});
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
    sh.loadFromFile("assets/shaders/grayscale.frag", sf::Shader::Fragment);
    float grayscale = 2.0;

    sh.setUniform("u_colorFactor", LinearInterpolation(grayscale, 0.f, 2.f, 0.f, 1.f));

    while (window.isOpen())
    {
        float lastframetime = mainRenderClock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event))
        {
            mainCharacter.HandleEvent(event);
            if (event.type == sf::Event::Closed)
            {
                std::cout << "Window closed\n";
                window.close();
            }
            if (event.type == sf::Event::Resized)
            {

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

                renderTexture.create(winWidth, winHeight);
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Q)
                {
                    if (event.key.control)
                    {
                        main_world.SaveChunks("DebugWorldSave");
                    }
                }
                if (event.key.code == sf::Keyboard::Equal)
                {
                    grayscale += 0.1;
                    grayscale = std::clamp(grayscale, 0.f, 2.f);
                    sh.setUniform("u_colorFactor", LinearInterpolation(grayscale, 0.f, 2.f, 0.f, 1.f));
                    // zoom *= 1.05;
                    // mainView.zoom(1.05);
                }
                if (event.key.code == sf::Keyboard::Dash)
                {
                    grayscale -= 0.1;
                    grayscale = std::clamp(grayscale, 0.f, 2.f);
                    sh.setUniform("u_colorFactor", LinearInterpolation(grayscale, 0.f, 2.f, 0.f, 1.f));
                    // zoom /= 1.05;
                    // mainView.zoom(1 / 1.05);
                }
                if (event.key.code == sf::Keyboard::F3 || event.key.code == sf::Keyboard::BackSlash)
                {
                    debug = !debug;
                }
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                {
                    if (event.mouseWheelScroll.delta < 0)
                    {
                        zoom *= 1.05;
                        mainView.zoom(1.05);
                    }
                    else if (event.mouseWheelScroll.delta > 0)
                    {
                        zoom /= 1.05;
                        mainView.zoom(1 / 1.05);
                    }
                }
            }
            //// if (event.type == sf::Event::MouseMoved) {
            ////     sh.setUniform("mouse_pos", sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
            //// }
        }

        // Invoke Updates
        mainCharacter.Update(lastframetime);
        sf::Vector2f pos = mainCharacter.GetPosition();
        sf::Vector2f currentChunkPosition = {std::floor(pos.x / kChunkSize), std::floor(pos.y / kChunkSize)};

        for (int o_x = -RENDER_DISTANCE; o_x <= RENDER_DISTANCE; o_x++)
        {
            for (int o_y = -RENDER_DISTANCE; o_y <= RENDER_DISTANCE; o_y++)
            {
                sf::Vector2i currentChunk = sf::Vector2i(currentChunkPosition) + sf::Vector2i(o_x, o_y);
                if (!main_world.IsChunkExist(currentChunk))
                {
                    main_world.PlaceChunk(currentChunk);
                    main_world.GenerateChunk(currentChunk);
                }
            }
        }

        // Clear Window
        window.clear(sf::Color::Black);

        // Set positioned view
        window.setView(sf::View({0, 0}, {static_cast<float>(winWidth), static_cast<float>(winHeight)}));

        // Draw game staff
        mainRenderWorld.Update(main_world, texture_manager);
        mainRenderWorld.setPosition(sf::Vector2f(-pos.x * TILE_SIZE, -pos.y * TILE_SIZE));

        renderTexture.setView(mainView);
        renderTexture.clear();
        renderTexture.draw(mainRenderWorld);
        renderTexture.display();

        sf::Sprite sprite;
        sprite.setTexture(renderTexture.getTexture());
        sprite.setPosition({static_cast<float>(-winWidth) / 2, static_cast<float>(-winHeight) / 2});

        window.draw(sprite, &sh);

        renderTexture.clear(sf::Color::Transparent);
        // Draw debug staff
        if (debug)
        {
            sf::Vector2f manualRenderOffset = {-pos.x * TILE_SIZE, -pos.y * TILE_SIZE};
            sf::Vertex quad[] = {
                sf::Vertex(
                    sf::Vector2f(
                        manualRenderOffset.x + currentChunkPosition.x * kChunkSize * TILE_SIZE,
                        manualRenderOffset.y + currentChunkPosition.y * kChunkSize * TILE_SIZE),
                    sf::Color::Red),
                sf::Vertex(
                    sf::Vector2f(
                        manualRenderOffset.x + (currentChunkPosition.x + 1) * kChunkSize * TILE_SIZE,
                        manualRenderOffset.y + currentChunkPosition.y * kChunkSize * TILE_SIZE),
                    sf::Color::Blue),
                sf::Vertex(
                    sf::Vector2f(
                        manualRenderOffset.x + (currentChunkPosition.x + 1) * kChunkSize * TILE_SIZE,
                        manualRenderOffset.y + (currentChunkPosition.y + 1) * kChunkSize * TILE_SIZE),
                    sf::Color::Red),
                sf::Vertex(
                    sf::Vector2f(
                        manualRenderOffset.x + currentChunkPosition.x * kChunkSize * TILE_SIZE,
                        manualRenderOffset.y + (currentChunkPosition.y + 1) * kChunkSize * TILE_SIZE),
                    sf::Color::Blue),
                sf::Vertex(
                    sf::Vector2f(
                        manualRenderOffset.x + currentChunkPosition.x * kChunkSize * TILE_SIZE,
                        manualRenderOffset.y + currentChunkPosition.y * kChunkSize * TILE_SIZE),
                    sf::Color::Red),
            };
            renderTexture.draw(quad, 5, sf::LineStrip);
        }
        renderTexture.display();

        sprite.setTexture(renderTexture.getTexture());
        window.draw(sprite);

        // Set inteface view
        window.setView(textView);

        // Draw debug staff
        if (debug)
        {
            std::stringstream debug_string;
            debug_string << "FPS: " << 1.0 / lastframetime << '\n';
            debug_string << "POS: "
                         << "X:" << mainCharacter.GetPosition().x << " Y:" << mainCharacter.GetPosition().y << '\n';
            debug_string << "shader_grayscale_value: " << grayscale / 2 << '\n';
            debug_string << "zoom: " << zoom << '\n'; 
            text.setString(debug_string.str());
            window.draw(text);
        }

        // Display everything drawn
        window.display();
    }
    std::cout << "Exiting Game\n";
    // mainWorld.SaveChunks("World1");
    return 0;
}
