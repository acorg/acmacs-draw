#pragma once

#include "acmacs-base/color.hh"
#include "acmacs-base/size-scale.hh"
#include "acmacs-draw/viewport.hh"

namespace acmacs::surface
{
    class Surface;
}

// ----------------------------------------------------------------------

void geographic_map_draw(acmacs::surface::Surface& aSurface, Color outline_color, Pixels outline_width);
acmacs::Size geographic_map_size();
acmacs::Viewport geographic_map_viewport();
double geographic_map_aspect();

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
