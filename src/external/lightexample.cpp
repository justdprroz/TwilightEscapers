#include <SFML/Graphics.hpp>
#include <external/FastNoiseLite.h>
#include <iostream>
#include <cmath>

class Application
{
public:
    Application() :
        m_rectangle{ { 100.f, 100.f } },
        m_position{ 0.f, 0.f }
    {
        m_mainView.setSize(m_winWidth, m_winHeight);
        m_mainView.setCenter(m_winWidth / 2, m_winHeight / 2);

        sf::ContextSettings settings;
        settings.antialiasingLevel = 16;
        m_window.create(sf::VideoMode(800, 800), "hello", sf::Style::Default, settings);
        m_window.setMouseCursorVisible(false);
        m_layer = new sf::RenderTexture();
        m_layer->create(800, 800, settings);
        flashlight_s = std::sqrt(800*800*2);
        sf::RenderTexture * mm_l; 
        std::cout << flashlight_s << '\n';
        m_flashlightTexture.create(400, 400, settings);
        generateSpot(m_darkness);

        m_flashlight.setTexture(m_flashlightTexture.getTexture(), true);
        m_flashlight.setOrigin({ 380, 200});
        m_flashlight.setPosition(400, 400);

        m_rectangle.setFillColor(sf::Color::Red);
        m_rectangle.setPosition(300,200);

        m_sprite.setTexture(m_layer->getTexture());
        m_sprite.setOrigin({400.f, 400.f});
        m_sprite.setPosition({400.f, 400.f});
    }

    void generateSpot(int d)
    {
        m_flashlightTexture.clear(sf::Color(0, 0, 0, 255));

        int il = 400;
        float itl = 200.f / 400;
        for (auto i = 0u; i < il; ++i)
        {
            float index = static_cast<float>(i);
            float radius = 200.f - (index * itl);
            auto temp = sf::CircleShape{ radius, 1000};
            temp.setOrigin(radius, radius);
            float alpha = 255 - (std::pow((255 - radius) / 255, d)) * 255 / d;
            std::cout << radius << " " << alpha << '\n';
            temp.setFillColor({ 0u, 0u, 0u, static_cast<sf::Uint8>(alpha)});
            temp.setPosition({ 400 - radius, 200});
            // temp.setPosition({ 0, 0});

            m_flashlightTexture.draw(temp, sf::BlendNone);
        }
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0, 200),sf::Color::Blue),
            sf::Vertex(sf::Vector2f(400, 200),sf::Color::Red),
            sf::Vertex(sf::Vector2f(380, 0),sf::Color::Green),
            sf::Vertex(sf::Vector2f(380, 500),sf::Color::Yellow)
        };
        // m_flashlightTexture.draw(line, 4, sf::Lines);
        m_flashlightTexture.display();
        m_flashlightTexture.getTexture().copyToImage().saveToFile("dumpfiles/flashlightrender.png");
    }

    void run()
    {
        sf::Font debugfont;
        debugfont.loadFromFile("assets/fonts/CascadiaCode.ttf");
        sf::Text text;
        text.setFont(debugfont);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        // text.setOutlineColor(sf::Color::White);
        // text.setOutlineThickness(1);
        sf::Clock mainRenderClock;
        sf::Sprite cursor;
        sf::Texture cursortexture;
        cursortexture.loadFromFile("assets/textures/gui/cursors.png");
        cursor.setTexture(cursortexture);
        cursor.setTextureRect(sf::IntRect(0, 0, 16, 16));
        cursor.setOrigin(0, 0);
        cursor.setScale({2, 2});
        while (m_window.isOpen()){   
            float lastframetime = mainRenderClock.restart().asSeconds();
            for (auto event = sf::Event{}; m_window.pollEvent(event);){
                if (event.type == sf::Event::Closed){
                    m_window.close();
                }
                if (event.type == sf::Event::Resized){
                    m_winWidth = event.size.width;
                    m_winHeight = event.size.height;
                    m_mainView.setSize(sf::Vector2f(m_winWidth, m_winHeight));
                    m_mainView.setCenter(sf::Vector2f(m_winWidth / 2, m_winHeight / 2));
                    std::cout << m_layer->getSize().x << ' ' << m_layer->getSize().y << '\n';
                    delete m_layer;
                    m_layer = new sf::RenderTexture();
                    m_layer->create(m_winWidth, m_winHeight);
                    m_sprite.setTexture(m_layer->getTexture(), true);
                    m_window.setView(m_mainView);
                }
                if (event.type == sf::Event::KeyPressed){
                    if (event.key.code == sf::Keyboard::W){
                        m_pressed[0] = true;
                    }
                    if (event.key.code == sf::Keyboard::D){
                        m_pressed[1] = true;
                    }
                    if (event.key.code == sf::Keyboard::S){
                        m_pressed[2] = true;
                    }
                    if (event.key.code == sf::Keyboard::A){
                        m_pressed[3] = true;
                    }
                    if (event.key.code == sf::Keyboard::Equal){
                        m_darkness++;
                        generateSpot(m_darkness);
                    }
                    if (event.key.code == sf::Keyboard::Dash){
                        m_darkness--;
                        generateSpot(m_darkness);
                    }
                }
                if (event.type == sf::Event::KeyReleased){
                    if (event.key.code == sf::Keyboard::W){
                        m_pressed[0] = false;
                    }
                    if (event.key.code == sf::Keyboard::D){
                        m_pressed[1] = false;
                    }
                    if (event.key.code == sf::Keyboard::S){
                        m_pressed[2] = false;
                    }
                    if (event.key.code == sf::Keyboard::A){
                        m_pressed[3] = false;
                    }
                    if (event.key.code == sf::Keyboard::Q){
                        m_layer->getTexture().copyToImage().saveToFile("dumpfiles/LastestRenderMask.png");
                        cursor.getTexture()->copyToImage().saveToFile("dumpfiles/cursor.png");
                    }
                }
            }
            m_pos.x += ((int)m_pressed[1] - (int)m_pressed[3]) * 300 * lastframetime;
            m_pos.y += ((int)m_pressed[2] - (int)m_pressed[0]) * 300 * lastframetime;
            // rotate flashlight
            sf::Vector2f m_center = m_flashlight.getPosition();
            m_position = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));

            cursor.setPosition(m_position);

            float nx = m_position.x - m_center.x;
            float ny = m_center.y - m_position.y;
            float angle = atan2(nx, ny);
            m_flashlight.setRotation(angle * 57.2958f + 90);
            m_flashlight.setPosition(m_pos);

            sf::CircleShape player(10);
            player.setOrigin(10,10);
            player.setPosition(m_center);

            // Clear
            m_layer->clear(sf::Color(0, 0, 0, 255));
            m_window.clear(sf::Color::White);
            
            // Draw anything
            // m_layer->draw(circle, sf::BlendNone);

            sf::RectangleShape shadow(m_rectangle);
            shadow.setFillColor(sf::Color(0, 0, 0, 0));

            sf::CircleShape playershadow(player);
            playershadow.setTexture(&m_flashlightTexture.getTexture());
            playershadow.setTextureRect(sf::IntRect(370, 190, 20, 20));
            playershadow.setRotation(angle * 57.2958f + 90);

            m_window.draw(m_rectangle);
            m_window.draw(player);

            // Draw with light
            m_layer->draw(m_flashlight, sf::BlendNone);
            // m_layer->draw(shadow, sf::BlendNone);
            // m_layer->draw(playershadow, sf::BlendNone);
            m_layer->display();
            m_window.draw(m_sprite);
            m_window.draw(cursor);

            text.setString(std::to_string(1.0 / lastframetime) + "\n\n" + std::to_string(m_darkness));
            sf::FloatRect backgroundRect = text.getLocalBounds();
            sf::RectangleShape background(sf::Vector2f(backgroundRect.width + 5, backgroundRect.height + 5));
            background.setFillColor(sf::Color(0, 0, 0, 128));
            m_window.draw(background, text.getTransform());
            m_window.draw(text);
            m_window.display();
        }
    }

private:
    sf::RenderWindow m_window;
    sf::RenderTexture m_flashlightTexture;
    sf::RenderTexture* m_layer;
    sf::RectangleShape m_rectangle;
    sf::Sprite m_flashlight;
    sf::Sprite m_sprite;
    sf::Vector2f m_pos = {100, 100};
    int flashlight_s;
    int m_darkness = 3;
    sf::Vector2f m_position;
    int m_winWidth = 800;
    int m_winHeight = 800;
    sf::View m_mainView;
    bool m_pressed[4] = {false, false, false, false};
};

int main()
{
    auto app = Application{};
    app.run();
}