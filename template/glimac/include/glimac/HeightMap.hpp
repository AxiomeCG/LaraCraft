//
// Created by Matthias Hudelot on 01/01/2020.
//

#ifndef TEMPLATE_HEIGHTMAP_HPP
#define TEMPLATE_HEIGHTMAP_HPP

#include "FilePath.hpp"
#include <vector>


namespace glimac {

class HeightMap {
public :
    HeightMap(unsigned int width, unsigned int height, float resw, float resh, float resv) :
            m_width(width), m_height(height), m_resw(resw), m_resh(resh), m_resv(resv),
            m_heightData(width * resw, std::vector<float>(height * resh)) {

    }

    unsigned int getWidth() const {
        return m_width;
    }

    unsigned int getHeight() const {
        return m_height;
    }

    void insert(unsigned int i, unsigned int j, float value) {
        m_heightData[i][j] = value;
    }

    std::vector<std::vector<float>> getHeightData() {
        return m_heightData;
    }


private :
    unsigned int m_width;
    unsigned int m_height;
    float m_resw;
    float m_resh;
    std::vector<std::vector<float>> m_heightData;
    float m_resv;

};

std::unique_ptr<HeightMap> loadHeightMap(const FilePath &filepath, float resw, float resh, float resv);

}

#endif //TEMPLATE_HEIGHTMAP_HPP
