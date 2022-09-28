#include <SFML/Graphics.hpp>
#include <Utils.hpp>
#include <NoiseOld.hpp>
#include <sstream>
#include <iostream>
#include <Entity.hpp>
#include <Character.hpp>
#include <World.hpp>
#include <Map.hpp>
#include <Block.hpp>

// define variables

vec2f mousePos = vec2f(0, 0);
siv::PerlinNoise perlin(15000);
int mouseButton = -1;
bool released;
sf::Event g_curEvent;
bool newEvent;
int state = 0;
int seed = 1800;
int const MAPsize = 1 << 10;
float scale = 1;
int tilesize = 16;
int MAP[MAPsize][MAPsize];
sf::Texture textures[16];

// Character player;

std::pair<float, float> playerPos = {2, 2};
sf::Texture playerTextureFront;
sf::Texture playerTextureBack;
sf::Texture playerTexture;

sf::Clock clockS;
float zoom = 1.f;
int width = 800, height = 800;
// float dvx = width / 2 * zoom - (width / 2) + playerPos.first * tilesize * scale;
// float dvy = height / 2 * zoom - (height / 2) + playerPos.second * tilesize * scale;

// float plOnScX = playerPos.first * tilesize * scale - dvx;
// float plOnScY = playerPos.second * tilesize * scale - dvy;

bool Wpress, Apress, Spress, Dpress;
int speed = 10;
sf::View view;
float plOnWinX = 0;
float plOnWinY = 0;
int mapshiftoffset = 100;
sf::Font font;
std::stringstream logger;
int tileStartY = 0, tileStartX = 0, tileStopY = 0, tileStopX = 0;
bool debug = 0;
int dirtcount = 0;
float power = 2;

// some gui setup

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

// button callback functions

int play(){
    state = 2;
    return 0;
}

int settings(){
    state = 1;
    return 0;
}

// texture loading

void loadTextures(){
    bool result;
    // std::cout << "Loading textures:\n";
    // std::cout << "Loading tiles\n";
    // result = textures[0].loadFromFile("textures/tiles/normal/stone.png");
    // std::cout << "Stone: " << result << '\n';
    // result = textures[1].loadFromFile("textures/tiles/normal/grass.png");
    // std::cout << "Grass: " << result << '\n';
    // std::cout << "loading entities textures\n";
    result = playerTextureFront.loadFromFile("textures/main/Entity/guard.png", sf::IntRect(0, 0, 32, 32));
    std::cout << "Guard front: " << result << '\n';
    result = playerTextureBack.loadFromFile("textures/main/Entity/guard.png", sf::IntRect(32, 0, 32, 32));
    std::cout << "Guard back: " << result << '\n';
    playerTexture = playerTextureFront;
    // std::cout << "Textures loaded\n";
}

// main menu screen

void MainMenu(sf::RenderWindow &win) {
    // create view
    sf::View dv(sf::Vector2f(width / 2, height / 2),sf::Vector2f(width, height));
    // dv.move(-100, -100);
    win.setView(dv);

    
    // create button context
    buttonContext bc;
    
    // play button

    bc.pos = vec2f(0,0);
    bc.size = vec2f(192,108);
    bc.window = &win;
    bc.function = &play;

    // load button textures
    sf::Texture toload;
    toload.loadFromFile("textures/main/Gui/playButton.png");
    bc.defaultTexture = sf::Texture(toload);
    toload.loadFromFile("textures/main/Gui/playButtonHover.png");
    bc.hoverTexture = sf::Texture(toload);
    toload.loadFromFile("textures/main/Gui/playButtonPressed.png");
    bc.clickedTexture = sf::Texture(toload);

    // trigger button
    button(bc);
    
    // setting button

    bc.pos = vec2f(0,108);
    bc.size = vec2f(192,108);
    bc.window = &win;
    bc.function = &settings;
    
    // load button textures
    toload.loadFromFile("textures/main/Gui/settingsButton.png");
    bc.defaultTexture = sf::Texture(toload);
    toload.loadFromFile("textures/main/Gui/settingsButtonHover.png");
    bc.hoverTexture = sf::Texture(toload);
    toload.loadFromFile("textures/main/Gui/settingsButtonPressed.png");
    bc.clickedTexture = sf::Texture(toload);

    // trigger button
    button(bc);
}

// debug functions

// map borders to move
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

// logger

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

// player hitbox highlight

void DebugCurrentTile(sf::RenderWindow &win){
    view.setSize(sf::Vector2f(width, height));
    // view.setCenter(sf::Vector2f(width / 2 + dvx, height / 2 + dvy));
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
    cell.setPosition(flx * tilesize * scale, fly * tilesize * scale);
    cell.setSize({tilesize * scale, tilesize * scale});
    win.draw(cell);
}

// some fucking useless stuff

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

// main game drawing

// draw entities

void drawEntities(sf::RenderWindow &win){
    float locscaleX = (float)tilesize / (float)playerTexture.getSize().x;
    float locscaleY = (float)tilesize / (float)playerTexture.getSize().y;
    sf::Sprite cell;
    cell.setTexture(playerTexture);
    cell.setPosition(playerPos.first * tilesize * scale, playerPos.second * tilesize * scale);
    cell.setScale(sf::Vector2f(locscaleX * scale,locscaleY * scale));
    win.draw(cell);
}

// draw tiles

void drawTiles(sf::RenderWindow &win) {
    sf::Sprite cell;
    cell.setScale(sf::Vector2f(scale,scale));
    for(int i = std::max(tileStartX - 1, 0); i <= std::min(tileStopX + 1, MAPsize - 1); i++){
        for(int i1 = std::max(tileStartY - 1, 0); i1 <= std::min(tileStopY + 1, MAPsize - 1); i1++){
            int curBlock = MAP[i][i1];
            cell.setTexture(textures[curBlock]);
            int dis = distance<int>(playerPos.first, playerPos.second, i, i1);
            if (debug == 0){
                cell.setColor(sf::Color(255, 255, 255, std::max(255 - int(dis * 20 / power), 0)));
            }
            cell.setPosition(i * tilesize * scale, i1 * tilesize * scale);
            win.draw(cell);
        }
    }
}

// draw HUD

void drawHUD(sf::RenderWindow &win){
    sf::View dv(sf::Vector2f(width / 2, height / 2),sf::Vector2f(width, height));
    win.setView(dv);
    sf::Text text;
    std::string dct = "Dirt Count: ";
    dct += std::to_string(dirtcount);
    text.setString(dct);
    text.setFont(font);
    text.setPosition(0, height - 20);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setOutlineColor(sf::Color::White);
    text.setOutlineThickness(1);
    win.draw(text);
}

void GameScreen(sf::RenderWindow &win){
    // view.setSize(sf::Vector2f(width, height));
    // view.setCenter(sf::Vector2f(width / 2 + dvx, height / 2 + dvy));
    // view.zoom(zoom);
    // win.setView(view);
    // drawTiles(win);
    // drawEntities(win);
    // drawHUD(win);
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

    std::cout << "app started" << '\n';

    World world;
    Character player;

    std::cout << playerTextureFront.getSize().x << '\n';
    world.attachPlayer(&player);

    std::cout << font.loadFromFile("arial.ttf") << '\n';

    loadTextures();
    // generateMap();

    player.setTextureBack(playerTextureBack);
    player.setTextureFront(playerTextureFront);
    
    sf::RenderWindow window(sf::VideoMode(width, height), "EscapeFromTwilight", sf::Style::Default);
    window.setView(view);

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
            player.handleEvents(event);
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
                // view.setCenter(sf::Vector2f(width / 2 + dvx, height / 2 + dvy));
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
                if (event.key.code == sf::Keyboard::F3){
                    debug = (debug + 1) % 2;
                }
                if (event.key.code == sf::Keyboard::Equal){
                    power *= 2;
                }
                if (event.key.code == sf::Keyboard::Dash){
                    power /= 2;
                }
            }
            if (event.type == sf::Event::KeyReleased){
                if (event.key.code == sf::Keyboard::Escape){
                    if (state != 0){
                        state = 0;
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
            state = 0;
        }
        //  main game screen
        if (state == 2){
            world.step(lastframetime);
            world.draw(window);

            // character.move(lastframetime);
            // playerPos = character.m_position;

            
            // int velX = 0;
            // int velY = 0;

            // get input
            // if (Wpress){
            //     playerTexture = playerTextureBack;
            //     velY -= speed;
            // }
            // if (Spress){
            //     playerTexture = playerTextureFront;
            //     velY += speed;
            // }
            // if (Dpress){
            //     velX += speed;
            // }
            // if (Apress){
            //     velX -= speed;
            // }

            // float dX = velX * lastframetime, dY = velY * lastframetime;

            // move if on map
            // if (playerPos.first + dX >= 0 && playerPos.first + dX <= MAPsize - 0.9f){
            //     playerPos.first += dX;
            // } else {
            //     dX = 0;
            // }
            // if (playerPos.second + dY >= 0 && playerPos.second + dY <= MAPsize - 0.9f){
            //     playerPos.second += dY;
            // } else {
            //     dY = 0;
            // }

            // return to map if outside
            // playerPos.first = std::min(std::max(0.f, playerPos.first), (float)(MAPsize - 0.9f));
            // playerPos.second = std::min(std::max(0.f, playerPos.second), (float)(MAPsize - 0.9f));

            // get sprite coords on screen
            // plOnWinX = width / 2 - (width / 2 - plOnScX) / zoom;
            // plOnWinY = height / 2 - (height / 2 - plOnScY) / zoom;

            // tileStartX = (playerPos.first * tilesize * scale - plOnWinX * zoom) / (tilesize * scale);
            // tileStartY = (playerPos.second * tilesize * scale - plOnWinY * zoom) / (tilesize * scale);
            // tileStopX = tileStartX + width * zoom / (tilesize * scale);
            // tileStopY = tileStartY + height * zoom / (tilesize * scale);

            // logger << "Player: " << '\n';
            // logger << "\tx: " << playerPos.first << '\n';
            // logger << "\ty: " << playerPos.second << '\n';
            // logger << "Window: " << '\n';
            // logger << "\tPlayerOnScreen: " << '\n';
            // logger << "\t\tx: " << plOnWinX << '\n';
            // logger << "\t\ty: " << plOnWinY << '\n';
            // logger << "\tIndent: " << '\n';
            // logger << "\t\tx: " << dvx << '\n';
            // logger << "\t\ty: " << dvy << '\n';
            // logger << "\tCorner: " << '\n';
            // logger << "\t\tx: " << width / 2 - (width / 2) / zoom - dvx / zoom << '\n';
            // logger << "\t\ty: " << height / 2 - (height / 2) / zoom - dvy / zoom << '\n';
            logger << "FPS: " << 1 / lastframetime << '\n';

            // shift map
            // if (plOnWinX < mapshiftoffset && dX < 0 && tilesize * scale * playerPos.first > mapshiftoffset * zoom ||
            //     plOnWinX > width - mapshiftoffset && dX > 0 && tilesize * scale * playerPos.first < MAPsize * tilesize * scale - mapshiftoffset * zoom){
            //     dvx += dX * tilesize * scale;
            // } else {
            //     plOnScX += dX * tilesize * scale;
            // }
            // if (plOnWinY < mapshiftoffset && dY < 0 && tilesize * scale * playerPos.second > mapshiftoffset * zoom ||
            //     plOnWinY > height - mapshiftoffset && dY > 0 && tilesize * scale * playerPos.second < MAPsize * tilesize * scale - mapshiftoffset * zoom){
            //     dvy += dY * tilesize * scale;
            // } else {
            //     plOnScY += dY * tilesize * scale;
            // }

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