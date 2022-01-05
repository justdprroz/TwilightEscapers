#include <SFML/Graphics.hpp>
#include <external/FastNoiseLite.h>
#include <iostream>

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
        settings.antialiasingLevel = 8;
        m_window.create(sf::VideoMode(800, 800), "hello", sf::Style::Default, settings);
        m_layer.create(800, 800);
        flashlight_s = std::sqrt(800*800*2);
        std::cout << flashlight_s << '\n';
        m_flashlightTexture.create(flashlight_s, flashlight_s);
        generateSpot();

        m_flashlight.setTexture(m_flashlightTexture.getTexture(), true);
        m_flashlight.setOrigin({ flashlight_s / 2.f, flashlight_s / 2.f });
        m_flashlight.setPosition(400, 400);

        m_rectangle.setFillColor(sf::Color::Red);
        m_rectangle.setPosition(300,200);

        m_sprite.setTexture(m_layer.getTexture());
        m_sprite.setOrigin({400.f, 400.f});
        m_sprite.setPosition({400.f, 400.f});
    }

    void generateSpot()
    {
        m_flashlightTexture.clear(sf::Color(0, 0, 0, 227));

        int il = 400;
        float itl = 200.f / 400;
        for (auto i = 0u; i < il; ++i)
        {
            float index = static_cast<float>(i);
            float radius = 200.f - (index * itl);
            auto temp = sf::CircleShape{ radius, 1000};
            temp.setOrigin(radius, radius);
            temp.setFillColor({ 0u, 0u, 0u, static_cast<sf::Uint8>(255 - (255 - radius) / 2)});
            temp.setPosition({ flashlight_s / 2.f - radius, flashlight_s / 2.f});
            // temp.setPosition({ 0, 0});

            m_flashlightTexture.draw(temp, sf::BlendNone);
        }

        m_flashlightTexture.display();
        m_flashlightTexture.getTexture().copyToImage().saveToFile("flashlightrender.png");
    }

    void run()
    {
        sf::Font debugfont;
        debugfont.loadFromFile("arial.ttf");
        sf::Text text;
        text.setFont(debugfont);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::Black);
        text.setOutlineColor(sf::Color::White);
        text.setOutlineThickness(1);
        sf::Clock mainRenderClock;
        while (m_window.isOpen())
        {   
            
            for (auto event = sf::Event{}; m_window.pollEvent(event);)
            {
                if (event.type == sf::Event::Closed)
                {
                    m_window.close();
                }
                if (event.type == sf::Event::Resized){
                    m_winWidth = event.size.width;
                    m_winHeight = event.size.height;
                    m_mainView.setSize(sf::Vector2f(m_winWidth, m_winHeight));
                    m_mainView.setCenter(sf::Vector2f(m_winWidth / 2, m_winHeight / 2));
                    m_window.setView(m_mainView);
                }
            }
            // rotate flashlight
            sf::Vector2f m_center = m_flashlight.getPosition();
            m_position = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
            float nx = m_position.x - m_center.x;
            float ny = m_center.y - m_position.y;
            float angle = atan2(nx, ny);
            m_flashlight.setRotation(angle * 57.2958f + 90);
            m_layer.clear();
            m_layer.draw(m_flashlight, sf::BlendNone);
            m_layer.display();

            m_window.clear(sf::Color::Blue);

            m_window.draw(m_rectangle);
            m_window.draw(m_sprite);

            float lastframetime = mainRenderClock.restart().asSeconds();
            text.setString(std::to_string(1.0 / lastframetime));
            m_window.draw(text);

            m_window.display();
        }
    }

private:
    sf::RenderWindow m_window;
    sf::RenderTexture m_flashlightTexture;
    sf::RenderTexture m_layer;
    sf::RectangleShape m_rectangle;
    sf::Sprite m_flashlight;
    sf::Sprite m_sprite;
    int flashlight_s;
    sf::Vector2f m_position;
    int m_winWidth = 800;
    int m_winHeight = 800;
    sf::View m_mainView;
};

int main()
{
    auto app = Application{};
    app.run();
}