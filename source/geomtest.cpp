#include <SFML/Graphics.hpp>

int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(800, 800), "hello", sf::Style::Default, settings);

    sf::Clock cl;

    sf::RectangleShape rect;
    rect.setSize({300, 150});
    rect.setOrigin(150, 75);
    rect.setPosition({150, 300});
    while (window.isOpen()) {
        float frametime = cl.restart().asSeconds();
        window.setTitle(std::to_string(1.0f / frametime));
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        rect.rotate(5.0f * frametime);
        window.clear(sf::Color::Black);
        window.draw(rect);
        window.display();
    }
    return 0;
}