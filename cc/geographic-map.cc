#include "acmacs-draw/geographic-map.hh"
#include "acmacs-draw/geographic-path.hh"
#include "acmacs-draw/surface.hh"

// ----------------------------------------------------------------------

void geographic_map_draw(Surface& aSurface)
{
    const auto& path = geographic_path::geographic_map_path();
    Color geographic_map_outline_color("black");
    Pixels geographic_map_outline_width{1};
    aSurface.path_outline(path.first, path.second, geographic_map_outline_color, geographic_map_outline_width);

} // geographic_map_draw

// ----------------------------------------------------------------------

Size geographic_map_size()
{
    return {geographic_path::geographic_map_size[0], geographic_path::geographic_map_size[1]};

} // geographic_map_size

// ----------------------------------------------------------------------

double geographic_map_aspect()
{
    return geographic_path::geographic_map_size[0] / geographic_path::geographic_map_size[1];

} // geographic_map_aspect

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
