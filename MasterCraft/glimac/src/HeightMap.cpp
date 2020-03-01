//
// Created by Matthias Hudelot on 01/01/2020.
//

#include "glimac/HeightMap.hpp"
#include <cmath>
#include "stb_image.h"

namespace glimac {

    std::unique_ptr <HeightMap> loadHeightMap(const FilePath &filepath, float resw, float resh, float resv) {
        int x, y, n;
        unsigned char *data = stbi_load(filepath.c_str(), &x, &y, &n, 1);
        if (!data) {
            std::cerr << "loading image " << filepath << " error: " << stbi_failure_reason() << std::endl;
            return std::unique_ptr<HeightMap>();
        }
        std::unique_ptr <HeightMap> pHeightMap(new HeightMap(x, y, resw, resh, resv));
        //auto ptr = pHeightMap->getHeightData();
        int k = 0;
        for (auto i = 0u; i < x; ++i) {
            for (auto j = 0u; j < y; ++j) {
                //std::cout << (float)data[k] * resv << std::endl;
                pHeightMap->insert(i, j, floor(((float) data[k]) * resv));
                ++k;
            }
        }
        stbi_image_free(data);
        return pHeightMap;
    }

}