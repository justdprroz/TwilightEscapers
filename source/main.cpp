#include <SFML/Graphics.hpp>
#include <iostream>
#include "utils.hpp"
#include "noise.hpp"
#include <sstream>

vec2f mousePos = vec2f(0, 0);
int mouseButton = -1;
bool released;
sf::Event g_curEvent;
bool newEvent;
int state = 0;

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
float zoom = 2.f;
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
bool debug = 0;
unsigned dirtcount = 0;

struct buttonContext {
    vec2f pos;
    vec2f size;
    sf::Texture defaultTexture;
    sf::Texture hoverTexture;
    sf::Texture clickedTexture;
    int (*function)();
    sf::RenderWindow* window;
};

void handleEvent(sf::Event &e){
    if (e.type == sf::Event::MouseMoved){
        mousePos = {(float)e.mouseMove.x, (float)e.mouseMove.y};
    }
    if (e.type == sf::Event::MouseButtonPressed){
        mouseButton = e.mouseButton.button;
    }
    if (e.type == sf::Event::MouseButtonReleased){
        mouseButton = -1;
        released = true;
    }
}

bool button(buttonContext cont){
    // Draw
    sf::Texture active = cont.defaultTexture;
    if (inRange<float>(cont.pos.x, cont.pos.x + cont.size.x, mousePos.x) && inRange<float>(cont.pos.y, cont.pos.y + cont.size.y, mousePos.y)){
        active = cont.hoverTexture;
    }
    if (inRange<float>(cont.pos.x, cont.pos.x + cont.size.x, mousePos.x) && inRange<float>(cont.pos.y, cont.pos.y + cont.size.y, mousePos.y) && mouseButton > -1){
        active = cont.clickedTexture;
    }
    if (g_curEvent.type == sf::Event::MouseButtonReleased){
        if (inRange<float>(cont.pos.x, cont.pos.x + cont.size.x, g_curEvent.mouseButton.x) && inRange<float>(cont.pos.y, cont.pos.y + cont.size.y, g_curEvent.mouseButton.y)){
            cont.function();
            active = cont.hoverTexture;
        }
    }
    sf::Sprite toDraw;
    toDraw.setTexture(active);
    toDraw.setPosition(cont.pos.x, cont.pos.y);
    sf::Vector2u texSize;
    texSize = active.getSize();
    toDraw.setScale(sf::Vector2f(cont.size.x / texSize.x, cont.size.y / texSize.y));
    cont.window->draw(toDraw);
    // Return if pressed
    return false;
}

int play(){
    state = 2;
    return 0;
}

int settings(){
    state = 1;
    return 0;
}

int distance(int x1, int y1, int x2, int y2)
{
    // Calculating distance
    return (int)std::sqrt(std::pow(x2 - x1, 2) +
                    std::pow(y2 - y1, 2) * 1.0);
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
    sf::View dv(sf::Vector2f(width / 2, height / 2),sf::Vector2f(width, height));
    win.setView(dv);
    sf::Text text;
    text.setString(logger.str());
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setOutlineColor(sf::Color::White);
    text.setOutlineThickness(1);
    win.draw(text);
}

void PickUpDirt(){
    int ilx = (int)playerPos.first;
    int ily = (int)playerPos.second;
    for(int x = std::max(ilx, 0); x <= std::min(MAPsize, ilx + 1); x++){
        for(int y = std::max(ily, 0); y <= std::min(MAPsize, ily + 1); y++){
            if (MAP[x][y] == 1){
                MAP[x][y] = 0;
                dirtcount++;
            }
        }
    }
}

void DebugCurrentTile(sf::RenderWindow &win){
    view.setSize(sf::Vector2f(width, height));
    view.setCenter(sf::Vector2f(width / 2 + dvx, height / 2 + dvy));
    view.zoom(zoom);
    win.setView(view);
    float flx = playerPos.first;
    float fly = playerPos.second;
    int ilx = (int)flx;
    int ily = (int)fly;
    for(int x = std::max(ilx, 0); x <= std::min(MAPsize, ilx + 1); x++){
        for(int y = std::max(ily, 0); y <= std::min(MAPsize, ily + 1); y++){
            sf::RectangleShape cell;
            cell.setFillColor(sf::Color(255, 0, 0, 64));
            cell.setPosition(x * tilesize * scale, y * tilesize * scale);
            cell.setSize({tilesize * scale, tilesize * scale});
            win.draw(cell);
        }
    }

    sf::RectangleShape cell;
    cell.setFillColor(sf::Color(0, 0, 255, 64));
    cell.setPosition(flx, fly);
    cell.setSize({tilesize * scale, tilesize * scale});
    win.draw(cell);
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
    for(int i = std::max(tileStartX - 1, 0); i <= std::min(tileStopX + 1, MAPsize - 1); i++){
        for(int i1 = std::max(tileStartY - 1, 0); i1 <= std::min(tileStopY + 1, MAPsize - 1); i1++){
            int curBlock = MAP[i][i1];
            sf::Sprite cell;
            cell.setTexture(textures[curBlock]);
            int dis = distance(playerPos.first, playerPos.second, i, i1);
            cell.setColor(sf::Color(255, 255, 255, std::max(255 - (int)(std::sqrt(dis) * 50), 0)));
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


    std::cout << font.loadFromFile("arial.ttf") << '\n';

    loadTextures();
    generateMap();

    sf::RenderWindow window(sf::VideoMode(width, height), "EscapeFromTwilight", sf::Style::Default);
    window.setView(view);

    std::cout << "hello! press enter to continue" << "\n";
    std::cin;


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
                    if (state == 2){
                        Dpress = true;
                    }
                }
                if (event.key.code == sf::Keyboard::A){
                    Apress = true;
                }
                if (event.key.code == sf::Keyboard::F3){
                    debug = (debug + 1) % 2;
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
            PickUpDirt();
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

            tileStartX = (playerPos.first * tilesize * scale - plOnWinX * zoom) / (tilesize * scale);
            tileStartY = (playerPos.second * tilesize * scale - plOnWinY * zoom) / (tilesize * scale);
            tileStopX = tileStartX + width * zoom / (tilesize * scale);
            tileStopY = tileStartY + height * zoom / (tilesize * scale);

            logger << "Player:\n\tx: " << playerPos.first << "\n\ty: " << playerPos.second << '\n';
            logger << "\twinX: " << plOnWinX << "\n\twinY " << plOnWinY << '\n';
            logger << "Tiles:\n\tUpperLeft:\n\t\tx: " << tileStartX << "\n\t\ty: " << tileStartY << '\n';
            logger << "\tLowerRight:\n\t\tx: " << tileStopX << "\n\t\ty: " << tileStopY << '\n';
            logger << "\tCurrent:\n\t\tx: " << playerPos.first << "\n\t\ty: " << playerPos.second << '\n';
            logger << "\t\tBlock: " << MAP[(int)playerPos.first][(int)playerPos.second] << '\n';

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
            if (debug == 1){
                DebugMapShiftBorders(window);
                DebugCurrentTile(window);
                DebugLogger(window);
            }
        }
        window.display();
    }
}