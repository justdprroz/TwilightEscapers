#pragma once

#include <logic/base/Base.hpp>
#include <SFML/Graphics/Color.hpp>
#include <utility>

class Light : public Base {
public:
    Light(){
        m_className = "Light";
    };
protected:
    std::pair<float, float> m_position;
    float m_intensity;
    sf::Color m_color;
};