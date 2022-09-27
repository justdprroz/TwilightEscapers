#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>

enum RenderLayerType
{
    WORLD,
    GUI,
    DEBUG
};

struct RenderLayer
{
public:
    RenderLayer() = default;
    RenderLayer(float width, float height);
    void SetZoom(float zoom);
    sf::View m_view;
    float m_zoom;
};

class Renderer
{
public:
    Renderer() = default;
    Renderer(sf::Vector2u winSize);

    void RenderToTexture(RenderLayerType layer, const sf::Drawable &drawable);

private:
    sf::RenderTexture m_render_texture;
    RenderLayer m_render_layers[3];

    float m_scale = 1.0f;
};

#endif // RENDERER_HPP