#pragma once

#include <SFML/Graphics/Color.hpp>
#include <utility>

class Light {
public:
    Light(){
        
    };
protected:
    std::pair<float, float> m_position;
    float m_intensity;
    sf::Color m_color;
};