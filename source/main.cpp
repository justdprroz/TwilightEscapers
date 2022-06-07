// Copyright 2021-2022 JustDprroz

#include <iostream>
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <Render.hpp>
#include <utility>
#include <Utils.hpp>


const int TEXTURE_SIZE = 16;
const int TILE_SIZE = 32;//32



void Terminal() {

}

int main() {
    sf::Font debugfont;
    debugfont.loadFromFile("assets/fonts/CascadiaCode.ttf");

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
    sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), title, sf::Style::Default, settings);
    window.setView(mainView);
    window.setFramerateLimit(256); // check when you want
    // window.setVerticalSyncEnabled(true); // check when you want
    // bool fullscreen = false; // check when you want
    window.setKeyRepeatEnabled(false);

    // Clocks and time
    sf::Clock mainRenderClock;
    // sf::Clock mainTickClock;

    // World
    World main_world;
    main_world.SetSeed(420);
    main_world.NoiseInit();

    const int render_distance = 2;
    for(int i = -render_distance; i < render_distance; i++){
        for(int j = -render_distance; j < render_distance; j++){
            main_world.PlaceChunk({i, j});
            main_world.GenerateChunk({i, j});
        }
    }

    // Character
    Character mainCharacter(0, {0.0f, 0.0f});
    main_world.SummonEntity(mainCharacter);

    // Rendering
    TextureManager texture_manager("assets");
    RenderWorld mainRenderWorld;

    // Debug gui
    sf::Text text;
    text.setFont(debugfont);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setOutlineColor(sf::Color::White);
    text.setOutlineThickness(1);

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
        mainCharacter.Update(lastframetime);

        // RenderCharacter.Update(&c, texture_manager);

        sf::Vector2f pos = mainCharacter.GetPosition();

        window.setView(mainView);
        window.clear(sf::Color::Black);

        mainRenderWorld.Update(main_world, texture_manager);
        
        mainRenderWorld.setPosition(sf::Vector2f(-pos.x * TILE_SIZE, -pos.y * TILE_SIZE));
        // mainRenderWorld.rotate(10.0f * lastframetime);

        window.draw(mainRenderWorld);
        // cr.setPosition(sf::Vector2f(-pos.x * TILE_SIZE, -pos.y * TILE_SIZE));
        // window.draw(cr);
        

        text.setString(
            std::to_string(1.0 / lastframetime) + '\n' +
            std::to_string(mainCharacter.GetPosition().x) + " " + std::to_string(mainCharacter.GetPosition().y));

        window.setView(textView);
        window.draw(text);
        window.display();
        // std::cin.get();
    }

    // mainWorld.SaveChunks("World1");
    return 0;
}