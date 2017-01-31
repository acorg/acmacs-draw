#include "acmacs-draw/continent-map.hh"
#include "acmacs-draw/continent-path.hh"
#include "acmacs-draw/surface.hh"

// ----------------------------------------------------------------------

const char* const ContinentLabels[9] = {
    "EUROPE", "CENTRAL-AMERICA", "MIDDLE-EAST", "NORTH-AMERICA", "AFRICA", "ASIA", "RUSSIA", "AUSTRALIA-OCEANIA", "SOUTH-AMERICA" //, "ANTARCTICA"
};

#include "acmacs-base/global-constructors-push.hh"

static const std::map<std::string, Color> sContinentColors = {
    {"EUROPE",            0x00FF00},
    {"CENTRAL-AMERICA",   0xAAF9FF},
    {"MIDDLE-EAST",       0x8000FF},
    {"NORTH-AMERICA",     0x00008B},
    {"AFRICA",            0xFF8000},
    {"ASIA",              0xFF0000},
    {"RUSSIA",            0xB03060},
    {"AUSTRALIA-OCEANIA", 0xFF69B4},
    {"SOUTH-AMERICA",     0x40E0D0},
    {"ANTARCTICA",        0x808080},
    {"CHINA-SOUTH",       0xFF0000},
    {"CHINA-NORTH",       0x6495ED},
    {"CHINA-UNKNOWN",     0x808080},
    {"UNKNOWN",           0x808080},
};

#include "acmacs-base/diagnostics-pop.hh"

Color continent_color(std::string aContinent)
{
    try {
        return sContinentColors.at(aContinent);
    }
    catch (std::out_of_range&) {
        return sContinentColors.at("UNKNOWN");
    }
}

// ----------------------------------------------------------------------

void continent_map_draw(Surface& aSurface)
{
    for (const auto& continent: ContinentLabels) {
        const auto& path = continent_path::continent_map_path(continent);
          // aSurface.path_outline(path.first, path.second, geographic_map_outline_color, geographic_map_outline_width);
        aSurface.path_fill(path.first, path.second, continent_color(continent));
    }

} // continent_map_draw

// ----------------------------------------------------------------------

Size continent_map_size()
{
    return {continent_path::continent_map_size[0], continent_path::continent_map_size[1]};

} // continent_map_size

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
