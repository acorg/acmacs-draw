#pragma once

#include <map>

#include "acmacs-base/color.hh"
#include "acmacs-base/size.hh"

namespace acmacs::surface
{
    class Surface;
}

// ----------------------------------------------------------------------

extern const char* const ContinentLabels[9];

// Color continent_color(std::string aContinent);
void continent_map_draw(acmacs::surface::Surface& aSurface);
acmacs::Size continent_map_size();
double continent_map_aspect();

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
