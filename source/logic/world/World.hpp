#pragma once

#include <logic/base/Base.hpp>
#include <logic/world/Block.hpp>
#include <logic/world/Light.hpp>
#include <logic/world/Map.hpp>
#include <logic/entity/Entity.hpp>
#include <vector>
#include <SFML/Graphics/View.hpp>

class World : public Base {
public:
    World(){
        m_className = "World ";
    };
protected:
    Map m_mainMap;
    std::vector<Light> m_lightSources;
    std::vector<Entity*> m_entities;
    sf::View m_mainView;
};