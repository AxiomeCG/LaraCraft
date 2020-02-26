//
// Created by adam on 04/02/2020.
//

#ifndef IMACGL_TEXTUREENUM_H
#define IMACGL_TEXTUREENUM_H

/**
 * Represent an enum for the offset of the texture
 */
class TextureOffsetEnum {
public:
    /**
     * The number of texture
     */
    static constexpr float NUMBER_TEXTURE = 10.f;

    /**
     * Offset for the dirt texture
     */
    static const float dirtTextureOffset;

    /**
     * Offset for the sand texture
     */
    static const float sandTextureOffset;

    /**
     * Offset for the rock texture
     */
    static const float rockTextureOffset;

    /**
     * Offset for the wood texture
     */
    static const float woodTextureOffset;

    /**
     * Offset for the leaf texture
     */
    static const float leafTextureOffset;

    /**
     * Offset for the mush texture
     */
    static const float mushTextureOffset;

    /**
     * Offset for the snow texture
     */
    static const float snowTextureOffset;
};



#endif //IMACGL_TEXTUREENUM_H
