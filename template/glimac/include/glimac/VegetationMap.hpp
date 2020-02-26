//
// Created by Matthias Hudelot on 11/02/2020.
//

#ifndef TEMPLATE_VEGETATIONMAP_HPP
#define TEMPLATE_VEGETATIONMAP_HPP

#include "FilePath.hpp"
#include "glm.hpp"
#include <vector>
#include <iostream>
#include <memory>

namespace glimac {

    /**
     * Represent a Vegetation Map
     */
    class VegetationMap {
    public:
        /**
         * Constructor for the Vegetation Map
         * @param width Width of the VegetationMap
         * @param height Height of the VegetationMap
         * @param resw Resolution for the width
         * @param resh Resolution for the height
         * @param resv Resolution for the values of the VegetationMap
         */
        VegetationMap(unsigned int width, unsigned int height, float resw, float resh, float resv) :
                m_width(width), m_height(height), m_resw(resw), m_resh(resh), m_resv(resv),
                m_vegetationData(width * resw, std::vector<glm::vec3>(height * resh)) {

        }

        /**
         * Getter for the width of the VegetationMap
         * @return the width
         */
        unsigned int getWidth() const {
            return m_width;
        }

        /**
         * Getter for the height of the VegetationMap
         * @return the height
         */
        unsigned int getHeight() const {
            return m_height;
        }

        /**
         * Insert a vec3 in the vegetation map
         * @param i Index x of the VegetationMap
         * @param j Index z of the VegetationMap
         * @param r Value of red
         * @param g Value of green
         * @param b Value of the blue
         */
        void insert(unsigned int i, unsigned int j, float r, float g, float b) {
            m_vegetationData[i][j] = glm::vec3(r, g, b);
        }

        /**
         * Getter for the data of the VegetationMap
         * @return The data of the VegetationMap
         */
        std::vector<std::vector<glm::vec3>> getVegetationData() const {
            return m_vegetationData;
        }

    private:
        /**
         * Width of the VegetationMap
         */
        unsigned int m_width;

        /**
         * Height of the VegetationMap
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
         * The data of the VegetationMap
         */
        std::vector<std::vector<glm::vec3>> m_vegetationData;

        /**
         * Resolution of the values of the VegetationMap
         */
        float m_resv;

    };

    /**
     * Load the VegetationMap from the filepath
     * @param filepath Filepath of the image of the VegetationMap
     * @param resw Resolution of the width
     * @param resh Resolution of the height
     * @param resv Resolution of the values of the VegetationMap
     * @return A unique pointer of a VegetationMap
     */
    std::unique_ptr<VegetationMap> loadVegetationMap(const FilePath &filepath, float resw, float resh, float resv);

}

#endif //TEMPLATE_VEGETATIONMAP_HPP
