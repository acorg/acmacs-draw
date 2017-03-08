#pragma once

#include "acmacs-base/color.hh"
#include "acmacs-draw/size.hh"

class Surface;

// ----------------------------------------------------------------------

void geographic_map_draw(Surface& aSurface);
Size geographic_map_size();
double geographic_map_aspect();

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
