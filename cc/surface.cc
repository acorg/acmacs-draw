#include "acmacs-draw/surface.hh"

// ----------------------------------------------------------------------

void Surface::grid(Scaled aStep, Color aLineColor, Pixels aLineWidth)
{
    const Viewport& v = viewport();
    for (auto x = v.left_scaled() + aStep; x < v.right_scaled(); x += aStep)
        line({x, v.top_scaled()}, {x, v.bottom_scaled()}, aLineColor, aLineWidth);
    for (auto y = v.top_scaled() + aStep; y < v.bottom_scaled(); y += aStep)
        line({v.left_scaled(), y}, {v.right_scaled(), y}, aLineColor, aLineWidth);

} // Surface::grid

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
