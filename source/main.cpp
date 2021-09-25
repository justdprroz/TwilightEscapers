#include <SFML/Graphics.hpp>
#include <iostream>
#include "utils.hpp"
#include "gui.hpp"
#include "noise.hpp"
#include <sstream>

const siv::PerlinNoise perlin(time(NULL));
int const MAPsize = 1 << 6;
float scale = 2;
int tilesize = 16;
int MAP[MAPsize][MAPsize];
sf::Texture textures[16];
std::pair<float, float> playerPos = {10, 15};
sf::Texture playerTextureFront;
sf::Texture playerTextureBack;
sf::Texture playerTexture;
sf::Clock clockS;
float velX;
float velY;
float dvx = 0, dvy = 0;
bool Wpress, Apress, Spress, Dpress;
int speed = 10;
float zoom = 1.f;
sf::View view;
int width = 800, height = 800;
float plOnScX = playerPos.first * tilesize * scale;
float plOnScY = playerPos.second * tilesize * scale;
float plOnWinX = 0;
float plOnWinY = 0;
int mapshiftoffset = 100;
sf::Font font;
std::stringstream logger;
int tileStartY = 0, tileStartX = 0, tileStopY = 0, tileStopX = 0;

int play(){
    state = 2;
    return 0;
}

int settings(){
    state = 1;
    return 0;
}

void loadTextures(){
    std::cout << "Loading textures:\n";
    std::cout << "Loading tiles\n";
    std::cout << "Stone: " <<
        textures[0].loadFromFile("textures/tiles/normal/stone.png") << '\n';
    std::cout << "Grass: " <<
        textures[1].loadFromFile("textures/tiles/normal/grass.png") << '\n';
    std::cout << "loading entities textures\n";
    std::cout << "Guard front: " <<
        playerTextureFront.loadFromFile("textures/entities/guard.png", 
                                        sf::IntRect(0, 0, 32, 32)) << '\n';
    std::cout << "Guard back: " <<
        playerTextureBack.loadFromFile("textures/entities/guard.png",
                                        sf::IntRect(32, 0, 32, 32)) << '\n';
    playerTexture = playerTextureFront;
    std::cout << "Textures loaded\n";
}

void MainMenu(sf::RenderWindow &win) {
    // create view
    sf::View dv(sf::Vector2f(width / 2, height / 2),sf::Vector2f(width, height));
    win.setView(dv);

    // create button context
    buttonContext bc;
    bc.pos = vec2f(0,0);
    bc.size = vec2f(192,108);
    bc.window = &win;
    bc.function = &play;

    // load button textures
    sf::Texture toload;
    toload.loadFromFile("textures/playButton.png");
    bc.defaultTexture = sf::Texture(toload);
    toload.loadFromFile("textures/playButtonHover.png");
    bc.hoverTexture = sf::Texture(toload);
    toload.loadFromFile("textures/playButtonPressed.png");
    bc.clickedTexture = sf::Texture(toload);

    // trigger button
    button(bc);
    
    
    // create button context
    bc.pos = vec2f(0,108);
    bc.size = vec2f(192,108);
    bc.window = &win;
    bc.function = &settings;
    
    // load button textures
    toload.loadFromFile("textures/settingsButton.png");
    bc.defaultTexture = sf::Texture(toload);
    toload.loadFromFile("textures/settingsButtonHover.png");
    bc.hoverTexture = sf::Texture(toload);
    toload.loadFromFile("textures/settingsButtonPressed.png");
    bc.clickedTexture = sf::Texture(toload);

    // trigger button
    button(bc);
}

void DebugMapShiftBorders(sf::RenderWindow &win){
    sf::View dv(sf::Vector2f(width / 2, height / 2),sf::Vector2f(width, height));
    win.setView(dv);
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(mapshiftoffset, 0), sf::Color::Red),
        sf::Vertex(sf::Vector2f(mapshiftoffset, height), sf::Color::Red),
        sf::Vertex(sf::Vector2f(width - mapshiftoffset, 0), sf::Color::Blue),
        sf::Vertex(sf::Vector2f(width - mapshiftoffset, height), sf::Color::Blue),
        sf::Vertex(sf::Vector2f(0, mapshiftoffset), sf::Color::Red),
        sf::Vertex(sf::Vector2f(width, mapshiftoffset), sf::Color::Red),
        sf::Vertex(sf::Vector2f(0, height - mapshiftoffset), sf::Color::Blue),
        sf::Vertex(sf::Vector2f(width, height - mapshiftoffset), sf::Color::Blue)
    };
    win.draw(line, 8, sf::Lines);
}

void DebugLogger(sf::RenderWindow &win){
    sf::Text text;
    text.setString(logger.str());
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    win.draw(text);
}

void drawEntities(sf::RenderWindow &win){
    float locscaleX = (float)tilesize / (float)playerTexture.getSize().x;
    float locscaleY = (float)tilesize / (float)playerTexture.getSize().y;
    sf::Sprite cell;
    cell.setTexture(playerTexture);
    cell.setPosition(playerPos.first * tilesize * scale, playerPos.second * tilesize * scale);
    cell.setScale(sf::Vector2f(locscaleX * scale,locscaleY * scale));
    win.draw(cell);
}

void drawTiles(sf::RenderWindow &win) {
    for(int i = std::max(tileStartX, 0); i <= std::min(tileStopX, MAPsize - 1); i++){
        for(int i1 = std::max(tileStartY, 0); i1 <= std::min(tileStopY, MAPsize - 1); i1++){
            int curBlock = MAP[i][i1];
            sf::Sprite cell;
            cell.setTexture(textures[curBlock]);
            cell.setPosition(i * tilesize * scale, i1 * tilesize * scale);
            cell.setScale(sf::Vector2f(scale,scale));
            win.draw(cell);
        }
    }
}

void GameScreen(sf::RenderWindow &win){
    view.setSize(sf::Vector2f(width, height));
    view.setCenter(sf::Vector2f(width / 2 + dvx, height / 2 + dvy));
    view.zoom(zoom);
    win.setView(view);
    drawTiles(win);
    drawEntities(win);
}

void generateMap(){
    for(int i = 0; i < MAPsize; i++){
        for(int i1 = 0; i1 < MAPsize; i1++){
            int noise = (int)(perlin.accumulatedOctaveNoise2D_0_1(i * 4.0 / MAPsize, i1 * 4.0 / MAPsize, 8) * 8) % 2;
            MAP[i][i1] = noise;
        }
    }
}

int main(){
    // initialization
    loadTextures();
    generateMap();
    font.loadFromFile("arial.ttf");
    sf::RenderWindow window(sf::VideoMode(width, height), "EscapeFromTwilight", sf::Style::Default);
    window.setView(view);

    sf::RenderWindow debugWindow(sf::VideoMode(width, height), "Debug Tools", sf::Style::Default);

    while (window.isOpen())
    {   
        // frametimer for movemets & etc
        double lastframetime = clockS.getElapsedTime().asSeconds();
        clockS.restart();

        // clear logger
        logger.str(std::string());

        // event processing
        sf::Event event;
        while (window.pollEvent(event))
        {    
            handleEvent(event);
            g_curEvent = event;
            if (event.type == sf::Event::Closed){
                window.close();
                return 0;
            }
            if (event.type == sf::Event::Resized) {
                int scale = 1;
                width = event.size.width;
                height = event.size.height;
                view.setSize(sf::Vector2f(width, height));
                view.setCenter(sf::Vector2f(width / 2 + dvx, height / 2 + dvy));
                view.zoom(zoom);
                window.setView(view);
            }
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::W){
                    Wpress = true;
                }
                if (event.key.code == sf::Keyboard::S){
                    Spress = true;
                }
                if (event.key.code == sf::Keyboard::D){
                    Dpress = true;
                }
                if (event.key.code == sf::Keyboard::A){
                    Apress = true;
                }
            }
            if (event.type == sf::Event::KeyReleased){
                if (event.key.code == sf::Keyboard::Escape){
                    if (state != 0){
                        state = 0;
                    }
                    else if (state == 0){
                        window.close();
                        return 0;
                    }
                }
                if (event.key.code == sf::Keyboard::W){
                    Wpress = false;
                }
                if (event.key.code == sf::Keyboard::S){
                    Spress = false;
                }
                if (event.key.code == sf::Keyboard::D){
                    Dpress = false;
                }
                if (event.key.code == sf::Keyboard::A){
                    Apress = false;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed){
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                // std::cout << worldPos.x << " " << worldPos.y << '\n';
            }
        }

        // clear window
        window.clear();
        debugWindow.clear(sf::Color::White);

        // process game states
        
        // menu
        if (state == 0){
            MainMenu(window);
        }
        // settings
        if (state == 1){
            std::cout << "No settings features" << '\n';
        }
        //  main game screen
        if (state == 2){
            // move character
            // prepare variables
            velX = 0;
            velY = 0;

            // get input
            if (Wpress){
                playerTexture = playerTextureBack;
                velY -= speed;
            }
            if (Spress){
                playerTexture = playerTextureFront;
                velY += speed;
            }
            if (Dpress){
                velX += speed;
            }
            if (Apress){
                velX -= speed;
            }

            float dX = velX * lastframetime, dY = velY * lastframetime;

            // move if on map
            if (playerPos.first + dX >= 0 && playerPos.first + dX <= MAPsize - 0.9f){
                playerPos.first += dX;
            } else {
                dX = 0;
            }
            if (playerPos.second + dY >= 0 && playerPos.second + dY <= MAPsize - 0.9f){
                playerPos.second += dY;
            } else {
                dY = 0;
            }

            // return to map if outside
            playerPos.first = std::min(std::max(0.f, playerPos.first), (float)(MAPsize - 0.9f));
            playerPos.second = std::min(std::max(0.f, playerPos.second), (float)(MAPsize - 0.9f));

            // get sprite coords on screen
            plOnWinX = width / 2 - (width / 2 - plOnScX) / zoom;
            plOnWinY = height / 2 - (height / 2 - plOnScY) / zoom;

            tileStartX = (playerPos.first * tilesize * scale - plOnWinX) / (tilesize * scale);
            tileStartY = (playerPos.second * tilesize * scale - plOnWinY) / (tilesize * scale);
            tileStopX = tileStartX + width / (tilesize * scale);
            tileStopY = tileStartY + height / (tilesize * scale);

            logger << "Player:\n\tx: " << playerPos.first << "\n\ty: " << playerPos.second << '\n';
            logger << "Tiles:\n\tUpperLeft:\n\t\tx: " << tileStartX << "\n\t\ty: " << tileStartY << '\n';
            logger << "\tLowerRight:\n\t\tx: " << tileStopX << "\n\t\ty: " << tileStopY << '\n';
            // shift map
            if (plOnWinX < mapshiftoffset && dX < 0 && tilesize * scale * playerPos.first > mapshiftoffset * zoom ||
                plOnWinX > width - mapshiftoffset && dX > 0 && tilesize * scale * playerPos.first < MAPsize * tilesize * scale - mapshiftoffset * zoom){
                dvx += dX * tilesize * scale;
            } else {
                plOnScX += dX * tilesize * scale;
            }
            if (plOnWinY < mapshiftoffset && dY < 0 && tilesize * scale * playerPos.second > mapshiftoffset * zoom ||
                plOnWinY > height - mapshiftoffset && dY > 0 && tilesize * scale * playerPos.second < MAPsize * tilesize * scale - mapshiftoffset * zoom){
                dvy += dY * tilesize * scale;
            } else {
                plOnScY += dY * tilesize * scale;
            }

            // Draw tiles and characters
            GameScreen(window);
            DebugMapShiftBorders(window);
            DebugLogger(debugWindow);
        }
        window.display();
        debugWindow.display();
    }
}