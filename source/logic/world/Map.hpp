#pragma once

#include <logic/world/Block.hpp>
#include <utils/NoiseOld.hpp>

class Map : public Base {
public:
    Map(){
        m_className = "Map";
    };
    void Generate (int p_seed){
        siv::PerlinNoise perlin(p_seed);
        for(int x = 0; x < m_sizeX; x++){
            for(int y = 0; y < m_sizeY; y++){
                int noise = (int)(perlin.accumulatedOctaveNoise2D_0_1(x * 4.0 / m_sizeX, y * 4.0 / m_sizeY, 8) * 8) % 2;
                if (noise == 0){
                    m_blocks[x][y] = new DirtBlock();
                }
                if (noise == 1){
                    m_blocks[x][y] = new StoneBlock();
                }
            }
        }
    }
    void Draw(sf::RenderWindow& p_win, int p_startX, int p_startY, int p_stopX, int p_stopY, const int p_tileSize){
        for (int x = p_startX; x < p_stopX; x++){
            for (int y = p_startX; y < p_stopX; y++){
                m_blocks[x][y]->Draw(p_win, x, y, p_tileSize);
            }
        }
    }
protected:
    static const int m_sizeX = 4096;
    static const int m_sizeY = 4096;
    Block* m_blocks[m_sizeX][m_sizeY];
};