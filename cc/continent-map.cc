#include "acmacs-base/color-continent.hh"
#include "acmacs-draw/continent-map.hh"
#include "acmacs-draw/continent-path.hh"
#include "acmacs-draw/surface.hh"

// ----------------------------------------------------------------------

const char* const ContinentLabels[9] = {
    "EUROPE", "CENTRAL-AMERICA", "MIDDLE-EAST", "NORTH-AMERICA", "AFRICA", "ASIA", "RUSSIA", "AUSTRALIA-OCEANIA", "SOUTH-AMERICA" //, "ANTARCTICA"
};

// ----------------------------------------------------------------------

void continent_map_draw(acmacs::surface::Surface& aSurface)
{
    for (const auto& continent: ContinentLabels) {
        const auto& path = continent_path::continent_map_path(continent);
          // aSurface.path_outline_negative_move(path.first, path.second, geographic_map_outline_color, geographic_map_outline_width);
        aSurface.path_fill_negative_move(path.first, path.second, acmacs::continent_color(continent));
    }

} // continent_map_draw

// ----------------------------------------------------------------------

acmacs::Size continent_map_size()
{
    return {continent_path::continent_map_size[0], continent_path::continent_map_size[1]};

} // continent_map_size

// ----------------------------------------------------------------------

double continent_map_aspect()
{
    return continent_path::continent_map_size[0] / continent_path::continent_map_size[1];

} // continent_map_aspect

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
