//
// Created by adam on 08/02/2020.
//

#ifndef IMACGL_OFFSETTEXTUREMAP_HPP
#define IMACGL_OFFSETTEXTUREMAP_HPP

#include "ColorMap.hpp"
#include "TextureOffsetEnum.hpp"


using namespace glimac;

/**
 * The aim of this class is to optimize the access to the right offset of the Texture while generating the chunks.
 * This map is supposed to be a translation of the color map in offsets.
 * The previous way was to check on generation if this was a Green, or Yellow or whatever color cube. That mean a test for each loop.
 * Due to pipeline architecture of the CPU, we were taking a lot of penalties with no real statistic heuristic about what color can show up.
 * Now we are treating only one time the color map to translate it as Texture Map that will have no check, just random access to a vector
 */
class OffsetTextureMap {
private:
    unsigned int width;
    unsigned int height;
    std::vector<std::vector<float>> offsetData;
public:
    explicit OffsetTextureMap(const ColorMap &colorMap);

    std::vector<std::vector<float>> getTextureData() const;

    unsigned int getWidth() const;

    unsigned int getHeight() const;

};

#endif //IMACGL_OFFSETTEXTUREMAP_HPP
