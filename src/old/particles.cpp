#include <SFML/Graphics.hpp>

sf::Clock colorclocks;

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:

    ParticleSystem(unsigned int count) :
    m_particles(count),
    m_vertices(sf::Points, count),
    m_lifetime(sf::seconds(3.f)),
    m_emitter(0.f, 0.f)
    {
    }

    void setEmitter(sf::Vector2f position)
    {
        m_emitter = position;
    }

    void update(sf::Time elapsed)
    {
        for (std::size_t i = 0; i < m_particles.size(); ++i)
        {
            // update the particle lifetime
            Particle& p = m_particles[i];
            p.lifetime -= elapsed;

            // if the particle is dead, respawn it
            if (p.lifetime <= sf::Time::Zero)
                resetParticle(i);

            // update the position of the corresponding vertex
            m_vertices[i].position += p.velocity * elapsed.asSeconds();

            // if(m_vertices[i].position.x < (0 + 50)) {
            //     p.velocity.x = std::abs(p.velocity.x);
            // }
            // if(m_vertices[i].position.x > (512 - 50)) {
            //     p.velocity.x = std::abs(p.velocity.x) * -1;
            // }

            // if(m_vertices[i].position.y < (0 + 50)) {
            //     p.velocity.y = std::abs(p.velocity.y);
            // }
            // if(m_vertices[i].position.y > (256 - 50)) {
            //     p.velocity.y = std::abs(p.velocity.y) * -1;
            // }
            // update the alpha (transparency) of the particle according to its lifetime
            float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
            m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
        }
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // our particles don't use a texture
        states.texture = NULL;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

private:

    struct Particle
    {
        sf::Vector2f velocity;
        sf::Time lifetime;
    };

    void resetParticle(std::size_t index)
    {
        // give a random velocity and lifetime to the particle
        float angle = (std::rand() % 360) * 3.14f / 180.f;
        float speed = (std::rand() % 50) + 50.f;
        m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        m_particles[index].lifetime = sf::milliseconds((std::rand() % 10000));

        // reset the position of the corresponding vertex
        m_vertices[index].position = sf::Vector2f(std::rand() % 512, std::rand() % 256);
        // m_vertices[index].color.r = static_cast<sf::Uint8>(std::rand() % 2 * 255);
        // m_vertices[index].color.g = static_cast<sf::Uint8>(std::rand() % 2 * 255);
        // m_vertices[index].color.b = static_cast<sf::Uint8>(std::rand() % 2 * 255);
    }

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    sf::Time m_lifetime;
    sf::Vector2f m_emitter;
};

int main() {
    sf::Font debugfont;
    debugfont.loadFromFile("assets/fonts/CascadiaCode.ttf");

    sf::View mainview;
    mainview.setSize(512, 256);
    mainview.setCenter(256, 128);

    sf::RenderWindow window(sf::VideoMode(1024, 512), "Particles");
    window.setMouseCursorVisible(false);
    ParticleSystem particles(1000);
    sf::Clock clock;
    sf::Clock RenderClock;
    sf::Text text;
    text.setFont(debugfont);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setOutlineColor(sf::Color::White);
    text.setOutlineThickness(1);
    while (window.isOpen())
    {
        float lastframetime = RenderClock.restart().asSeconds();
        // handle events
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.setView(mainview);
        // make the particle system emitter follow the mouse
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        particles.setEmitter(window.mapPixelToCoords(mouse));

        // update it
        sf::Time elapsed = clock.restart();
        particles.update(elapsed);

        window.clear();
        window.draw(particles);
        window.setView(sf::View());
        text.setString(
            std::to_string(1.0 / lastframetime) + '\n' + 
            std::to_string(colorclocks.getElapsedTime().asSeconds()) + '\n' + 
            std::to_string(window.mapPixelToCoords(mouse).x) + ' ' + std::to_string(window.mapPixelToCoords(mouse).y));
        window.draw(text);
        window.display();
    }

    return 0;
}