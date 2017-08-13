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

void Surface::double_arrow(const Location& a, const Location& b, Color aColor, Pixels aLineWidth, Pixels aArrowWidth)
{
    const bool x_eq = float_equal(b.x, a.x);
    const double sign2 = x_eq ? (a.y < b.y ? 1.0 : -1.0) : (b.x < a.x ? 1.0 : -1.0);
    const double angle = x_eq ? -M_PI_2 : std::atan((b.y - a.y) / (b.x - a.x));
    auto const la = arrow_head(a, angle, - sign2, aColor, aArrowWidth);
    auto const lb = arrow_head(b, angle,   sign2, aColor, aArrowWidth);
    line(la, lb, aColor, aLineWidth);

} // Surface::double_arrow

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
