// Copyright 2021 JustDprroz

#include <iostream>
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <utility>

int main() {
    Chunk c;
    c.SaveChunk("chunks");
    sf::RenderWindow window(sf::VideoMode(1000, 800), "EscapeFromTwilight", sf::Style::Default);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
    return 0;
}
