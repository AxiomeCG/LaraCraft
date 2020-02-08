//
// Created by Matthias Hudelot on 03/02/2020.
//

#include "glimac/ColorMap.hpp"
#include "stb_image.h"

namespace glimac {

    std::unique_ptr<ColorMap> loadColorMap(const FilePath &filepath, float resw, float resh, float resv) {
        int x, y, n;
        unsigned char *data = stbi_load(filepath.c_str(), &x, &y, &n, 3);
        if (!data) {
            std::cerr << "loading image " << filepath << " error: " << stbi_failure_reason() << std::endl;
            return std::unique_ptr<ColorMap>();
        }
        std::unique_ptr<ColorMap> pColorMap(new ColorMap(x, y, resw, resh, resv));
        for (auto i = 0u; i < x; ++i) {
            for (auto j = 0u; j < y; ++j) {
                auto offset = 3*(i + (j * x));
                auto r = data[offset] * resv;
                auto g = data[offset + 1] * resv;
                auto b = data[offset + 2] * resv;
                pColorMap->insert(i, j, r, g, b);
            }
        }
        stbi_image_free(data);
        return pColorMap;
    }

}
