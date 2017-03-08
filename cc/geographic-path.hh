#pragma once

#include <utility>

// ----------------------------------------------------------------------

namespace geographic_path
{
    constexpr const double geographic_map_size[2] = {1261.085, 632.59101};

    std::pair<const double*, const double*> geographic_map_path();
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
