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

    /**
     * Represent a Color Map
     */
    class ColorMap {

    public:

        /**
         * Constructor for the ColorMapp
         * @param width Width of the ColorMap
         * @param height Height of the ColorMap
         * @param resw Resolution for the width
         * @param resh Resolution for the height
         * @param resv Resolution for the values of the ColorMap
         */
        ColorMap(unsigned int width, unsigned int height, float resw, float resh, float resv) :
                m_width(width), m_height(height), m_resw(resw), m_resh(resh), m_resv(resv),
                m_colorData(width * resw, std::vector<glm::vec3>(height * resh)) {

        }

        /**
         * Getter for the width of the ColorMap
         * @return the width
         */
        unsigned int getWidth() const {
            return m_width;
        }

        /**
         * Getter for the height of the ColorMap
         * @return the height
         */
        unsigned int getHeight() const {
            return m_height;
        }

        /**
         * Insert a vec3 in the color map
         * @param i Index x of the ColorMap
         * @param j Index z of the ColorMap
         * @param r Value of red
         * @param g Value of green
         * @param b Value of the blue
         */
        void insert(unsigned int i, unsigned int j, float r, float g, float b) {
            m_colorData[i][j] = glm::vec3(r, g, b);
        }

        /**
         * Getter for the data of the ColorMap
         * @return The data of the ColorMap
         */
        std::vector<std::vector<glm::vec3>> getColorData() const {
            return m_colorData;
        }

    private:

        /**
         * Width of the ColorMap
         */
        unsigned int m_width;

        /**
         * Height of the ColorMap
         */
        unsigned int m_height;

        /**
         * Resolution of the width
         */
        float m_resw;

        /**
         * Resolution of the height
         */
        float m_resh;

        /**
         * The data of the ColorMap
         */
        std::vector<std::vector<glm::vec3>> m_colorData;

        /**
         * Resolution of the values of the ColorMap
         */
        float m_resv;

    };

    /**
     * Load the ColorMap from the filepath
     * @param filepath Filepath of the image of the ColorMap
     * @param resw Resolution of the width
     * @param resh Resolution of the height
     * @param resv Resolution of the values of the ColorMap
     * @return A unique pointer of a ColorMap
     */
    std::unique_ptr<ColorMap> loadColorMap(const FilePath &filepath, float resw, float resh, float resv);

}

#endif //TEMPLATE_COLORMAP_HPP
