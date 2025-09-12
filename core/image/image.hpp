#pragma once

#include "types.hpp"

#include <filesystem>

namespace Blackberry {

    Image LoadImageFromFile(const std::filesystem::path& path);

} // namespace Blackberry