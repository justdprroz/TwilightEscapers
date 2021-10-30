#pragma once

#include <logic/world/Block.hpp>
#include <utils/NoiseOld.hpp>

class Map {
public:
    Map(){
        
    };
    void generate(){
        m_blocks = new Block**[m_sizeX];
        for(int i = 0; i < m_sizeY; i++){
            m_blocks[i] = new Block*[m_sizeY];
        }
        siv::PerlinNoise l_perlin;
        for(int x = 0; x < m_sizeX; x++){
            for(int y = 0; y < m_sizeY; y++){
                int noise = (int)(l_perlin.accumulatedOctaveNoise2D_0_1(x * 4.0 / m_sizeX, y * 4.0 / m_sizeY, 8) * 8) % 2;
                if (noise == 0){
                    m_blocks[x][y] = new DirtBlock();
                }
                if (noise == 1){
                    m_blocks[x][y] = new StoneBlock();
                }
            }
        }
    }
    void draw(sf::RenderWindow& p_win, int p_startX, int p_startY, int p_stopX, int p_stopY, const int p_tileSize){
        for (int x = p_startX; x < p_stopX; x++){
            for (int y = p_startX; y < p_stopX; y++){
                m_blocks[x][y]->draw(p_win, x, y, p_tileSize);
            }
        }
    }
protected:
    static const int m_sizeX = 512;
    static const int m_sizeY = 512;
    Block*** m_blocks;
};