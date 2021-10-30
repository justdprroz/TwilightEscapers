#pragma once

#include <logic/world/Block.hpp>
#include <logic/world/Light.hpp>
#include <logic/world/Map.hpp>
#include <logic/entity/Entity.hpp>
#include <vector>
#include <SFML/Graphics/View.hpp>
#include <logic/entity/Character.hpp>

class World {
public:
    World(){
        m_mainMap.generate();
    };
    void draw(sf::RenderWindow &p_win){
        p_win.setView(m_mainView);
        m_mainMap.draw(p_win, m_upLeftTilePos.first, m_upLeftTilePos.second, m_downRightTilePos.first, m_downRightTilePos.second, m_tileSize);
        for(auto entity : m_entities){
            entity->draw(p_win, m_tileSize);
        };
        m_player->draw(p_win, m_tileSize);
    };
    void step(float p_timeStep){
        m_player->move(p_timeStep);
        for(auto entity : m_entities){
            entity->move(p_timeStep);
        };
    };
    void attachPlayer(Character* p_playerPtr){
        m_player = p_playerPtr;
    }
protected:
    Map m_mainMap;
    std::vector<Light> m_lightSources;
    std::vector<Entity*> m_entities;
    Character* m_player;
    sf::View m_mainView;
    int m_tileSize = 32;
    std::pair<float, float> m_viewOffset;
    std::pair<int, int> m_upLeftTilePos = {0, 0};
    std::pair<int, int> m_downRightTilePos = {30, 30};
};