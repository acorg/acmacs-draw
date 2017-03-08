#pragma once

#include "acmacs-base/color.hh"
#include "acmacs-base/size-scale.hh"
#include "acmacs-draw/viewport.hh"

class Surface;

// ----------------------------------------------------------------------

void geographic_map_draw(Surface& aSurface, Color outline_color, Pixels outline_width);
Size geographic_map_size();
Viewport geographic_map_viewport();
double geographic_map_aspect();

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
