#pragma once

#include <utility>

// ----------------------------------------------------------------------

namespace geographic_path
{
    constexpr const double geographic_map_size[2] = {1261.3, 632.591};

    std::pair<const double*, const double*> geographic_map_path();

    std::pair<double, double> calculate_geographic_map_size();
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
