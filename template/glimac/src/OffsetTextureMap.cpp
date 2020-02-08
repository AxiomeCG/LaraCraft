//
// Created by adam on 08/02/2020.
//

#include "OffsetTextureMap.hpp"


OffsetTextureMap::OffsetTextureMap(const ColorMap &colorMap) : width(colorMap.getWidth()), height(colorMap.getHeight()),
                                                               offsetData(colorMap.getColorData()
                                                                      .size()) {
    auto data = colorMap.getColorData();
    for (int i = 0; i < data.size(); i++) {
        std::vector<float> textureColumn(data[i].size(), TextureOffsetEnum::dirtTextureOffset);
        for (int j = 0; j < colorMap.getColorData()[i].size(); j++) {
            if (data[i][j] == glm::vec3(255, 255, 0)) {
                textureColumn[j] = TextureOffsetEnum::sandTextureOffset;
                continue;
            }
            if (data[i][j] == glm::vec3(128, 128, 128)) {
                textureColumn[j] = TextureOffsetEnum::rockTextureOffset;
                continue;
            }
            if (data[i][j] == glm::vec3(255, 0, 0)) {
                textureColumn[j] = TextureOffsetEnum::mushTextureOffset;
                continue;
            }
            if (data[i][j] == glm::vec3(255, 255, 255)) {
                textureColumn[j] = TextureOffsetEnum::snowTextureOffset;
                continue;
            }
        }
        offsetData[i] = textureColumn;
    }
}


unsigned int OffsetTextureMap::getWidth() const {
    return width;
}

unsigned int OffsetTextureMap::getHeight() const {
    return height;
}

std::vector<std::vector<float>> OffsetTextureMap::getTextureData() const {
    return offsetData;
}

