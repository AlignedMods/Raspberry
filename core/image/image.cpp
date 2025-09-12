#include "image/image.hpp"

#include "stb_image.h"

namespace Blackberry {

    Image LoadImageFromFile(const std::filesystem::path& path) {
        Image im{};

        im.data = stbi_load(path.string().c_str(), &im.width, &im.height, &im.channels, 4);

        return im;
    }

} // namespace Blackberry