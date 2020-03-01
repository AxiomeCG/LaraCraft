//
// Created by Matthias Hudelot on 01/01/2020.
//

#ifndef TEMPLATE_HEIGHTMAP_HPP
#define TEMPLATE_HEIGHTMAP_HPP

#include "FilePath.hpp"
#include <vector>
#include <iostream>
#include <memory>

namespace glimac {

    /**
     * Represent a Height Map
     */
    class HeightMap {
    public :

        /**
         * Constructor for the HeightMap
         * @param width Width of the HeightMap
         * @param height Height of the HeightMap
         * @param resw Resolution for the width
         * @param resh Resolution for the height
         * @param resv Resolution for the values of the HeightMap
         */
        HeightMap(unsigned int width, unsigned int height, float resw, float resh, float resv) :
                m_width(width), m_height(height), m_resw(resw), m_resh(resh), m_resv(resv),
                m_heightData(width * resw, std::vector<float>(height * resh)) {

        }

        /**
         * Getter for the width of the HeightMap
         * @return the width
         */
        unsigned int getWidth() const {
            return m_width;
        }

        /**
         * Getter for the height of the HeightMap
         * @return the height
         */
        unsigned int getHeight() const {
            return m_height;
        }

        /**
         * Insert a float in the height map
         * @param i Index x of the HeightMap
         * @param j Index z of the HeightMap
         * @param value Value to insert in the HeightMap
         */
        void insert(unsigned int i, unsigned int j, float value) {
            m_heightData[i][j] = value;
        }

        /**
         * Getter for the data of the HeightMap
         * @return The data of the HeightMap
         */
        std::vector<std::vector<float>> getHeightData() const {
            return m_heightData;
        }


    private :
        /**
         * Width of the HeightMap
         */
        unsigned int m_width;

        /**
         * Height of the HeightMap
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
         * The data of the HeightMap
         */
        std::vector<std::vector<float>> m_heightData;

        /**
         * Resolution of the values of the HeightMap
         */
        float m_resv;

    };

    /**
     * Load the HeightMap from the filepath
     * @param filepath Filepath of the image of the HeightMap
     * @param resw Resolution of the width
     * @param resh Resolution of the height
     * @param resv Resolution of the values of the HeightMap
     * @return A unique pointer of a HeightMap
     */
    std::unique_ptr<HeightMap> loadHeightMap(const FilePath &filepath, float resw, float resh, float resv);

}

#endif //TEMPLATE_HEIGHTMAP_HPP
