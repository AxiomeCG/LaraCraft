//
// Created by adam on 31/12/2019.
//

#ifndef IMACGL_CHUNK_HPP
#define IMACGL_CHUNK_HPP


/**
 * Represents a tower of block represented by a height and a texture.
 * Stores all the position of the blocks
 */
class Chunk {
private:
    /**
     * Number of blocks
     */
    int height;

    vector<glm::vec4> MMatrixList;
};


#endif //IMACGL_CHUNK_HPP
