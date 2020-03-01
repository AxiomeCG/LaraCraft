//
// Created by adam on 31/12/2019.
//

#ifndef IMACGL_CHUNKCOLUMN_HPP
#define IMACGL_CHUNKCOLUMN_HPP


#include <glm/vec4.hpp>
#include <vector>

/**
 * Represents a chunk "tower" represented by a height
 * Stores sections
 */
class ChunkColumn {
private:
    /**
     * Position of the highest block. Conditions the Chunk sections that will compose the chunk.
     * Must be between 0 and 255
     */
    int height;

    std::vector<glm::vec4> MMatrixList;
};


#endif //IMACGL_CHUNKCOLUMN_HPP
