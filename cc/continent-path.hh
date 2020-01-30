#pragma once

#include <utility>
#include <string>

// ----------------------------------------------------------------------

namespace continent_path
{
    // constexpr const double continent_map_size[2] = {686.475, 346.750};
    constexpr const double continent_map_size[2] = {660.0, 320.0};

    std::pair<const double*, const double*> continent_map_path(std::string continent);
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
