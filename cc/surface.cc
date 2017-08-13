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
    auto const la = arrow_head(a, angle, - sign2, aColor, aArrowWidth, true);
    auto const lb = arrow_head(b, angle,   sign2, aColor, aArrowWidth, true);
    line(la, lb, aColor, aLineWidth);

} // Surface::double_arrow

// ----------------------------------------------------------------------

Location Surface::arrow_head(const Location& arrow_end, double angle, double sign, Color aColor, Pixels aArrowWidth, bool aFilled)
{
    constexpr double ARROW_WIDTH_TO_LENGTH_RATIO = 2.0;

    const double arrow_width = aArrowWidth.value() / scale();
    const double arrow_length = arrow_width * ARROW_WIDTH_TO_LENGTH_RATIO;
    const Location line_attachment_point(arrow_end.x + sign * arrow_length * std::cos(angle), arrow_end.y + sign * arrow_length * std::sin(angle));
    const Location arrow_base_1(line_attachment_point.x + sign * arrow_width * std::cos(angle + M_PI_2) * 0.5, line_attachment_point.y + sign * arrow_width * std::sin(angle + M_PI_2) * 0.5);
    const Location arrow_base_2(line_attachment_point.x + sign * arrow_width * std::cos(angle - M_PI_2) * 0.5, line_attachment_point.y + sign * arrow_width * std::sin(angle - M_PI_2) * 0.5);

    const std::vector<Location> path{arrow_end, arrow_base_1, arrow_base_2};
    if (aFilled)
        path_fill(path.begin(), path.end(), aColor);
    else
        path_outline(path.begin(), path.end(), aColor, Pixels{1}, true, LineCap::Butt);

    return line_attachment_point;

} // Surface::arrow_head

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
