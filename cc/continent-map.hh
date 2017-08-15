#pragma once

#include <map>

#include "acmacs-base/color-target.hh"
#include "acmacs-draw/size.hh"

class Surface;

// ----------------------------------------------------------------------

extern const char* const ContinentLabels[9];

Color continent_color(std::string aContinent);
void continent_map_draw(Surface& aSurface);
Size continent_map_size();
double continent_map_aspect();

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
