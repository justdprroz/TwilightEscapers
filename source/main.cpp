// Copyright 2021 JustDprroz

#include <iostream>
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <Render.hpp>
#include <utility>
#include <Utils.hpp>

void Game(World &p_world, Renderer &p_renderer) {
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            p_renderer.SimpleRenderChunk(*(p_world.GetChunk({i, j})));
        }
    }
}

void Terminal() {

}

sf::Texture* CreateNoiseDebugTexture(int p_seed, int p_width, int p_height, FastNoiseLite::NoiseType p_type) {
    // create texture and helper image
    sf::Texture* texturePtr = new sf::Texture;
    sf::Image image;
    texturePtr->create(p_width, p_height);
    image.create(p_width, p_height);
    // create noise gen
    FastNoiseLite noise;
    noise.SetNoiseType(p_type);
    noise.SetFrequency(0.025);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(5);
    // put pixel on image
    for (int x = 0; x < p_width; x++) {
        for (int y = 0; y < p_height; y++) {
            float n = (noise.GetNoise((float)x, (float)y) + 1) / 2;
            image.setPixel(x, y, sf::Color(n * 256, n * 256 , n * 256, 255));
        }
    }
    // update texture and return its ptr
    texturePtr->update(image);
    return texturePtr;
}

sf::Texture* NoiseWorldMapAsTexture(int p_seed, int p_width, int p_height) {
    // create texture and helper image
    sf::Texture* texturePtr = new sf::Texture;
    sf::Image image;
    texturePtr->create(p_width, p_height);
    image.create(p_width, p_height);
    // create noise gen
    float freq = 0.005;

    FastNoiseLite noise;
    noise.SetSeed(p_seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    noise.SetFrequency(freq);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(3);

    // put pixel on image
    for (int x = 0; x < p_width; x++) {
        for (int y = 0; y < p_height; y++) {
            float n = LinearInterpolation<float>(noise.GetNoise((float)x, (float)y), -1, 1, 0, 1);
            sf::Uint8 opacity = (sf::Uint8)(n * 255);
            sf::Color blockcolor;
            if (InRange(n, 0.0f, 0.25f)) {
                opacity = (sf::Uint8)((n + 0.5f) * 255);
                blockcolor = {80, 172, 209, opacity};
            }
            else if (InRange(n, 0.25f, 0.3f)) {
                opacity = (sf::Uint8)((n - 0.25f + 0.5f) * 255);
                blockcolor = {219, 219, 134, opacity};
            }
            else if (InRange(n, 0.3f, 0.6f)) {
                opacity = (sf::Uint8)((n - 0.3f + 0.5f) * 255);
                blockcolor = {87, 217, 78, opacity};
            }
            else if (InRange(n, 0.6f, 1.1f)) {
                opacity = (sf::Uint8)((n - 0.6 + 0.5f) * 255);
                blockcolor = {89, 89, 89, opacity};
            }
            else {
                blockcolor = {255, 255, 255, opacity};
            }
            image.setPixel(x, y, blockcolor);
        }
    }
    // update texture and return its ptr
    texturePtr->update(image);
    return texturePtr;
}

void NoiseDebug(sf::RenderWindow &p_win, sf::Texture* p_ptrtext) {
    sf::Sprite cell;
    cell.setTexture(*p_ptrtext);
    cell.setPosition({0, 0});
    p_win.draw(cell);
}

int main() {
    // Some variables for window
    std::string title = "EscapeFromTwilight ";
    int winWidth = 1000;
    int winHeight = 800;
    float scale = 1;
    int viewWidth = winWidth * scale;
    int viewHeight = winHeight * scale;

    // View
    sf::View mainView;
    mainView.setSize(viewWidth, viewHeight);
    mainView.setCenter(viewWidth / 2, viewHeight / 2);
    // view.zoom();

    // Window
    sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), title, sf::Style::Default);
    window.setView(mainView);

    // Clock for frametime
    sf::Clock mainRenderClock;
    sf::Clock mainTickClock;

    // World
    World mainWorld;
    mainWorld.SetSeed(429);
    mainWorld.Init();

    // Renderer
    Renderer mainRenderer;
    mainRenderer.LoadTextures();
    mainRenderer.AttachWindow(&window);
    mainRenderer.AttachWorld(&mainWorld);

    // Noise Debug
    sf::Texture* worldMap;
    // worldMap = NoiseWorldMapAsTexture(42069, viewWidth, viewheight);
    // worldMap->copyToImage().saveToFile("noisedTexture.png");
    while (window.isOpen()) {
        float lastframetime = mainRenderClock.restart().asSeconds();

        window.setTitle(title + std::to_string(1.0 / lastframetime));

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::Resized){
                winWidth = event.size.width;
                winHeight = event.size.height;
                viewWidth = winWidth * scale;
                viewHeight = winHeight * scale;
                mainView.setSize(sf::Vector2f(viewWidth, viewHeight));
                mainView.setCenter(sf::Vector2f(viewWidth / 2, viewHeight / 2));
                window.setView(mainView);
            }
            if (event.type == sf::Event::KeyPressed){
            }
        }
        window.clear(sf::Color::Black);
        // NoiseDebug(window, worldMap);
        Game(mainWorld, mainRenderer);
        window.display();
    }

    // mainWorld.SaveChunks("World1");
    return 0;
}