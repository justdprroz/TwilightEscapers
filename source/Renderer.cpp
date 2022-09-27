#include "Renderer.hpp"

RenderLayer::RenderLayer(float width, float height)
{
    m_view = sf::View({0, 0}, {width, height});
    m_zoom = 1.f;
}

void RenderLayer::SetZoom(float zoom)
{
    m_view.zoom(1 / m_zoom);
    m_zoom = zoom;
    m_view.zoom(m_zoom);
}

Renderer::Renderer(sf::Vector2u winSize)
{
    m_render_texture.create(winSize.x, winSize.y);

    for (int i = 0; i < 3; i++)
    {
        m_render_layers[i] = RenderLayer(static_cast<float>(winSize.x), static_cast<float>(winSize.y));
    }
}

void Renderer::RenderToTexture(RenderLayerType layer, const sf::Drawable &drawable)
{
    m_render_texture.setView(m_render_layers[layer].m_view);
    m_render_texture.draw(drawable);
}