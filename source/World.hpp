#pragma once

#include <Block.hpp>
#include <Light.hpp>
#include <Map.hpp>
#include <Entity.hpp>
#include <vector>
#include <SFML/Graphics/View.hpp>
#include <Character.hpp>
#include <sstream>

extern std::stringstream logger;
class World {
public:
    World(){
        m_mainMap.generate();
        m_mainView.setSize(m_width, m_height);
        m_mainView.zoom(m_zoom);
        m_mainView.setCenter(sf::Vector2f(m_width / 2 + m_viewOffset.first, m_height / 2 + m_viewOffset.second));
    };
    void draw(sf::RenderWindow &p_win){
        m_mainView.setSize(m_width, m_height);
        m_mainView.zoom(m_zoom);
        m_mainView.setCenter(sf::Vector2f(m_width / 2 + m_viewOffset.first, m_height / 2 + m_viewOffset.second));
        p_win.setView(m_mainView);
        m_mainMap.draw(
            p_win, 
            m_upLeftTilePos.first - 1, 
            m_upLeftTilePos.second - 1, 
            m_downRightTilePos.first + 1, 
            m_downRightTilePos.second + 1, 
            m_tileSize
        );
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

        auto playerPos = m_player->getPosition();

        m_playerOnScreen.first = playerPos.first * m_tileSize - m_viewOffset.first;
        m_playerOnScreen.second = playerPos.second * m_tileSize - m_viewOffset.second;

        m_playerOnWindow.first = m_width / 2 - (m_width / 2 - m_playerOnScreen.first) / m_zoom;
        m_playerOnWindow.second = m_height / 2 - (m_height / 2 - m_playerOnScreen.second) / m_zoom;

        m_upLeftTilePos.first = (playerPos.first * m_tileSize - m_playerOnWindow.first * m_zoom) / (m_tileSize);
        m_upLeftTilePos.second = (playerPos.second * m_tileSize - m_playerOnWindow.second * m_zoom) / (m_tileSize);

        m_downRightTilePos.first = m_upLeftTilePos.first + m_width * m_zoom / m_tileSize;
        m_downRightTilePos.second = m_upLeftTilePos.second + m_height * m_zoom / m_tileSize;

        if (m_playerOnWindow.first < 100){
            m_viewOffset.first -= 100 - m_playerOnWindow.first;
        }
        if (m_playerOnWindow.first > 700){
            m_viewOffset.first += m_playerOnWindow.first - 700;
        }
        if (m_playerOnWindow.second < 100){
            m_viewOffset.second -= 100 - m_playerOnWindow.second;
        }
        if (m_playerOnWindow.second > 700){
            m_viewOffset.second += m_playerOnWindow.second - 700;
        }

        logger << "Player: " << '\n';
        logger << "\tx: " << playerPos.first << '\n';
        logger << "\ty: " << playerPos.second << '\n';
        logger << "Window: " << '\n';
        logger << "\tPlayerOnScreen: " << '\n';
        logger << "\t\tx: " << m_playerOnScreen.first << '\n';
        logger << "\t\ty: " << m_playerOnScreen.second << '\n';
        logger << "\tPlayerOnWindow: " << '\n';
        logger << "\t\tx: " << m_playerOnWindow.first << '\n';
        logger << "\t\ty: " << m_playerOnWindow.second << '\n';
        logger << "\tIndent: " << '\n';
        logger << "\t\tx: " << m_viewOffset.first << '\n';
        logger << "\t\ty: " << m_viewOffset.second << '\n';
        logger << "\tCorner: " << '\n';
        logger << "\t\tx: " << m_width / 2 - (m_width / 2) / m_zoom - m_viewOffset.first / m_zoom << '\n';
        logger << "\t\ty: " << m_height / 2 - (m_height / 2) / m_zoom - m_viewOffset.second / m_zoom << '\n';

    };

    void attachPlayer(Character* p_playerPtr){
        m_player = p_playerPtr;
    }
protected:
    Map m_mainMap;
    Character* m_player;
    int m_tileSize = 32;
    int m_width = 800;
    int m_height = 800;
    sf::View m_mainView;
    float m_zoom = 1.f;
    std::vector<Light> m_lightSources;
    std::vector<Entity*> m_entities;
    std::pair<float, float> m_viewOffset;
    std::pair<float, float> m_playerOnScreen;
    std::pair<float, float> m_playerOnWindow;
    std::pair<int, int> m_upLeftTilePos = {0, 0};
    std::pair<int, int> m_downRightTilePos = {0, 0};
};