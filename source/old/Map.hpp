#pragma once

#include <Block.hpp>
#include <NoiseOld.hpp>

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
        for (int x = std::max(0, p_startX); x < std::min(p_stopX, m_sizeX); x++){
            for (int y =std::max(0,  p_startY); y < std::min(p_stopY, m_sizeY); y++){
                m_blocks[x][y]->draw(p_win, x, y, p_tileSize);
            }
        }
    }
    std::pair<int, int> getSize(){
        return {m_sizeX, m_sizeY};
    }
protected:
    static const int m_sizeX = 512;
    static const int m_sizeY = 512;
    Block*** m_blocks;
};