#pragma once

#include <Block.hpp>
#include <Light.hpp>
#include <Map.hpp>
#include <Entity.hpp>
#include <vector>
#include <SFML/Graphics/View.hpp>
#include <Character.hpp>

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
    sf::View m_mainView;
    Character* m_player;
    int m_tileSize = 32;
    std::vector<Light> m_lightSources;
    std::vector<Entity*> m_entities;
    std::pair<float, float> m_viewOffset;
    std::pair<int, int> m_upLeftTilePos = {0, 0};
    std::pair<int, int> m_downRightTilePos = {30, 30};
};