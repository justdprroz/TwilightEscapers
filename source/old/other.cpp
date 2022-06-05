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