#pragma once

#include <logic/entity/Entity.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

class Character : public Entity {
public:
    Character() {
        m_className = "Character";
    };
    void handleEvents(sf::Event& p_event){
        if (p_event.type == sf::Event::KeyPressed){
            if (p_event.key.code == sf::Keyboard::W){
                m_pressed[0] = true;
            }
            if (p_event.key.code == sf::Keyboard::D){
                m_pressed[1] = true;
            }
            if (p_event.key.code == sf::Keyboard::S){
                m_pressed[2] = true;
            }
            if (p_event.key.code == sf::Keyboard::A){
                m_pressed[3] = true;
            }
        }
        if (p_event.type == sf::Event::KeyReleased){
            if (p_event.key.code == sf::Keyboard::W){
                m_pressed[0] = false;
            }
            if (p_event.key.code == sf::Keyboard::D){
                m_pressed[1] = false;
            }
            if (p_event.key.code == sf::Keyboard::S){
                m_pressed[2] = false;
            }
            if (p_event.key.code == sf::Keyboard::A){
                m_pressed[3] = false;
            }
        }
    };
    void move(float p_timeStep){
        m_velocity.first = ((int)m_pressed[1] - (int)m_pressed[3]) * m_speed;
        m_velocity.second = ((int)m_pressed[2] - (int)m_pressed[0]) * m_speed;
        std::cout << m_velocity.first << ' ' << m_velocity.second << '\n';
        Entity::move(p_timeStep);
    }
protected:
    bool m_pressed[4] = {false, false, false, false};
};