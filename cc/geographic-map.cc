#include "acmacs-draw/geographic-map.hh"
#include "acmacs-draw/geographic-path.hh"
#include "acmacs-draw/surface.hh"

// ----------------------------------------------------------------------

void geographic_map_draw(acmacs::surface::Surface& aSurface, Color outline_color, Pixels outline_width)
{
    const auto& path = geographic_path::geographic_map_path();
    aSurface.path_outline_negative_move(path.first, path.second, outline_color, outline_width);

} // geographic_map_draw

// ----------------------------------------------------------------------

acmacs::Size geographic_map_size()
{
    return {geographic_path::geographic_map_size[0], geographic_path::geographic_map_size[1]};

} // geographic_map_size

// ----------------------------------------------------------------------

double geographic_map_aspect()
{
    return geographic_path::geographic_map_size[0] / geographic_path::geographic_map_size[1];

} // geographic_map_aspect

// ----------------------------------------------------------------------

acmacs::Viewport geographic_map_viewport()
{
    using namespace geographic_path;
    return {acmacs::Location2D{geographic_map_bounds[0], -geographic_map_bounds[1]}, acmacs::Location2D{geographic_map_bounds[2], -geographic_map_bounds[3]}};

} // geographic_map_viewport

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
