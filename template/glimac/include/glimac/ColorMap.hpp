//
// Created by Matthias Hudelot on 03/02/2020.
//

#ifndef TEMPLATE_COLORMAP_HPP
#define TEMPLATE_COLORMAP_HPP

#include "FilePath.hpp"
#include "glm.hpp"
#include <vector>
#include <iostream>
#include <memory>

namespace glimac {

class ColorMap {

public:
    ColorMap(unsigned int width, unsigned int height, float resw, float resh, float resv) :
    m_width(width), m_height(height), m_resw(resw), m_resh(resh), m_resv(resv),
            m_colorData(width * resw, std::vector<glm::vec3>(height * resh)) {

    }

    unsigned int getWidth() const {
        return m_width;
    }

    unsigned int getHeight() const {
        return m_height;
    }

    void insert(unsigned int i, unsigned int j, float r, float g, float b) {
        m_colorData[i][j] = glm::vec3(r, g, b);
    }

    std::vector<std::vector<glm::vec3>> getColorData() const{
        return m_colorData;
    }

private:
    unsigned int m_width;
    unsigned int m_height;
    float m_resw;
    float m_resh;
    std::vector<std::vector<glm::vec3>> m_colorData;
    float m_resv;

};

    std::unique_ptr<ColorMap> loadColorMap(const FilePath &filepath, float resw, float resh, float resv);

}

#endif //TEMPLATE_COLORMAP_HPP
